#pragma once

#include <SDL2/SDL.h>

#include <cmath>
#include <cstdint>
#include <unordered_map>

#include "linalg.hpp"

namespace ThreeDL {
    using keymap_t = std::unordered_map<SDL_Keycode, bool>;

    class Camera {
        public:
            Camera(const vec3& position, const vec3& rotation);
            Camera(const vec3& position, const vec3& rotation, const float fov);
            Camera() = delete;

            vec3 position_;
            vec3 rotation_;
            vec3 forward_;
            vec3 right_;

            float fov_;

            void recalculateVectors();

            void moveForward(const float delta);
            void moveRight(const float delta);

            void rotate(const vec3 &delta);
            void tilt(const float delta);
            void pan(const float delta);
            void roll(const float delta);

            ~Camera() = default;

        private:
            vec3 max_rotation_ = {INFINITY, INFINITY, INFINITY};
    };

    class CameraController {
        public:
            explicit CameraController(Camera& camera);
            CameraController(const vec3& position, const vec3& rotation, const float fov);
            CameraController() = delete;

            int mouse_prev_x_;
            int mouse_prev_y_;
            int screen_centre_x_;
            int screen_centre_y_;
            bool rotation_enabled_ = false;

            void tick();

            void translate(const vec3& delta);
            void rotate(const vec3& delta);

            Camera& getCamera() const { return camera_; }

            ~CameraController() = default;

        private:
            Camera& camera_;
            Camera internal_camera_ = Camera {
                    {0, 0, 0},
                    {0, 0, 0},
                    36
            };

            keymap_t keymap_ = {};

            int mouse_x_;
            int mouse_y_;
    };
};