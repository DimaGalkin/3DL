#pragma once

#include <SDL2/SDL.h>

#include <cmath>

// cpp & hpp included so linking templates doesn't fail
#include "linalg.hpp"
#include "linalg.cpp"

#include "utils.hpp"

namespace ThreeDL {
    /**
     * @class Camera
     * @breif Represents a camera in 3D space. Provides methods to transform the camera.
    */
    class Camera {
        public:
            /**
             * @brief Creates a camera with a given position and rotation. Fov is set to default value.
            */
            Camera(const vec3& position, const vec3& rotation);

            /**
             * @brief Creates a camera with a given position, rotation and field of view.
            */
            Camera(const vec3& position, const vec3& rotation, const float fov);

            /**
             * @breif Do not allow camera to be created with blank parameters.
            */
            Camera() = delete;

            /**
             * @breif Recalculates the forward and right vectors based on the current rotation.
            */
            void recalculateVectors();

    				/**
						 * @breif Moves the camera forward by a given delta.
						*/
            void moveForward(const float delta);

            /**
             * @brief Moves the camera to the right by a given delta.
            */
            void moveRight(const float delta);

    				/**
						 * @brief Rotates the camera by a given angle. (Euler angles given in degrees)
						*/
            void rotate(const vec3 &delta);

    				/**
						 * @brief Pans the camera by a given angle given in degrees.
						*/
    				void tilt(const float delta);

            /**
             * @brief Rolls the camera by a given angle given in degrees.
            */
            void pan(const float delta);

    				/**
						 * @brief Rolls the camera by a given angle given in degrees.
						*/
            void roll(const float delta);

				    vec3 position_; // position of the camera in 3D space
				    vec3 rotation_; // rotation of the camera in 3D space (Euler angles)

				    // unit vectors representing direction camera moves in
				    vec3 forward_; // direction the camera is facing
				    vec3 right_; // direction to the right of the camera

				    float fov_; // field of view of the camera (in degrees)

            ~Camera() = default;

        private:
            vec3 max_rotation_ = {INFINITY, INFINITY, INFINITY}; // maximum rotation values
    };

    /**
     * @class CameraController
     * @brief Provides a way to control a camera using mouse and keyboard input.
    */
    class CameraController {
        public:
				    /**
				     * @brief Creates a controller for a given camera. Used when user requires to control an existing camera.
				    */
						explicit CameraController(Camera& camera);

				    /**
				     * @brief Creates a controller for a new camera. Used when user requires to create a new camera.
    				*/
				    CameraController(const vec3& position, const vec3& rotation, const float fov);

				    /**
				     * @breif Do not allow camera controller to be created with blank parameters.
				    */
				    CameraController() = delete;

				    /**
				     * @brief Called to update camera parameters from user input. Should be called every millisecond by user.
				    */
				    void tick();

				    /**
				     * @brief Translates the camera by a given delta.
				    */
				    void translate(const vec3& delta) const;

				    /**
				     * @brief Rotates the camera by a given delta. (Uses Euler angles in degrees.)
				    */
				    void rotate(const vec3& delta) const;

				    /**
				     * @brief Returns the camera being controlled by the controller.
				    */
				    Camera& getCamera() const { return camera_; }

    				// previous mouse position
    				ThreeDL::vec2i prev_mouse_pos_ = {0, 0};

    				// screen centre coordinates (to get mouse displacment from centre)
    				ThreeDL::vec2i screen_centre_ = {0, 0};

    				// allow user to disable mouse rotation (used for IMGUI)
    				bool rotation_enabled_ = false;

    				float rotation_speed_ = 50.0f; // speed at which camera rotates

            ~CameraController() = default;

        private:
    				// address of camera being controlled
            Camera& camera_;

    				// internal camera used when user creates a new camera if existing camera is not provided
            Camera internal_camera_ = Camera {
                    {0, 0, 0},
                    {0, 0, 0},
                    36
            };

    				// current mouse position
            int mouse_x_ = 0;
            int mouse_y_ = 0;
    };
};