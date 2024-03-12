#pragma once

#include <cmath>
#include <cstdint>

#include "linalg.hpp"

namespace ThreeDL {
    class Camera {
        public:
            Camera(const vec3& position, const vec3& rotation);
            Camera(const vec3& position, const vec3& rotation, const double fov);
            Camera() = delete;

            // expose internal vectors to allow user to perform custom calculations
            vec3 position_;
            vec3 rotation_;
            vec3 forward_;
            vec3 right_;

            double fov_;

            void recalculateVectors();

            void moveForward(const double delta);
            void moveRight(const double delta);

            void rotate(const vec3 &delta);
            void tilt(const double delta);
            void pan(const double delta);
            void roll(const double delta);

            ~Camera() = default;

        private:
            vec3 max_rotation_ = {INFINITY, INFINITY, INFINITY};
    };
};