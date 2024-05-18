#pragma once

#include <SDL2/SDL.h>
#include <cmath>

#include "utils.hpp"
#include "linalg.hpp"

namespace ThreeDL {
    /**
     * Camera class encapsulates parameters used to render a seen from it's point of view. Camera class is passed
     * to the renderer by reference so the user has the ability to change the camera classes parameters and see a
     * live update of the scene.
     *
     * If changing position_ or rotation_ without the use of the helper functions, recalculateVectors() must be
     * called to ensure that the forward_ and right_ vectors are correct.
     * Maximum rotation values also have no effect if rotation_ is changed without the use of the helper functions.
    */
    class Camera {
        public:
            Camera(const vec3& position, const vec3& rotation);
            Camera(const vec3& position, const vec3& rotation, const float fov);
            Camera() = delete;

            /**
             * Recalculates the forward_ and right_ vectors based on the current rotation_ values. These are unit
             * vectors that represent the directions that the camera moves in.
             * Must be called after changing rotation_ or position_. Methods of camera that change rotation_ or position_
             * call this method automatically.
            */
            void recalculateVectors();

            /**
             * Moves camera forward in the direction forward_ by delta units. Negative delta can be used to move the
             * camera backwards.
            */
            void moveForward(const float delta);

    				/**
						 * Moves camera right in the direction right_ by delta units. Negative delta can be used to move the
						 * camera to the left.
						*/
            void moveRight(const float delta);

            /**
             * Rotates the camera in a specified rotation order (see linalg.hpp) by delta degrees. vec3 of delta
             * represnts a set of Euler angles.
            */
            void rotate(const vec3& delta);

            /**
             * Tilt, pan and roll are used to rotate the camera in a specific direction by delta degrees. Negative delta
             * can be used to acomplish an opposite rotation.
            */
            void tilt(const float delta); // tilt is rotation around the y axis
            void pan(const float delta); // pan is rotation around the x axis
            void roll(const float delta); // roll is rotation around the z axis

				    vec3 position_;
				    vec3 rotation_; // Euler angles in degrees

				    // unit vectors representing direction camera moves in
				    vec3 forward_;
				    vec3 right_;

				    float fov_; // field of view of the camera (in degrees 0 < fov < 180)

            ~Camera() = default;

        private:
            vec3 max_rotation_ = {INFINITY, INFINITY, INFINITY}; // maximum rotation values
    };

    /**
     * CameraController class provides a way to control a camera object. CameraController used SDL2 to get mouse and
     * keyboard input which subsequently moves the camera. CameraController can take a reference to a camera object
     * so the user can control an existing camera or on can be created if the user chooses to not provide one.
    */
    class CameraController {
        public:
						explicit CameraController(Camera& camera);
				    CameraController(const vec3& position, const vec3& rotation, const float fov);
				    CameraController() = delete;

						/**
						 * Tick takes user input and applies neccessary transformations to the camera object. It is recomened to
						 * call this method multiple times per frame to ensure smooth camera movement.
						*/
						void tick();

						/**
						 * Translate and rotate are used to provide a quick way to move the camera without being required to retrive
						 * the reference. obj.translate(x, y, z) is the same as obj.getCamera().translate(x, y, z).
						*/
						void translate(const vec3& delta) const;
				    void rotate(const vec3& delta) const;

						/**
						 * Retrives reference that the user passed to the CameraController in the constructor. If the user did not
						 * pass a reference the one of the internal camera object is returned.
						*/
						Camera& getCamera() const { return camera_; }

    				// mouse position when camera controller was last ticked
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
                    75
            };

    				// current mouse position
            ThreeDL::vec2i mouse_pos_ = {0, 0};
    };
};