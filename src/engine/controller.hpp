#pragma once

#include <SDL2/SDL.h>

#include "camera.hpp"
#include "linalg.hpp"

namespace ThreeDL {
    class CameraController {
        public:
            explicit CameraController(Camera& camera);
            CameraController(const vec3& position, const vec3& rotation, const float fov);
            CameraController() = delete;

            void tick(double delta_time);

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
    };
};