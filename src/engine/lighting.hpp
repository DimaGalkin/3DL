#pragma once

#include "GPU/types.h"
#include "linalg.hpp"
#include "objects.hpp"
#include "utils.hpp"

namespace ThreeDL {
    class Light {
        public:
            Light();

            GPULight asGPUType() const;

            Object model_;

            LightType type_;
            vec3 position_;
            vec3 direction_;
            float intensity_;
            float color_[3];

            ~Light() = default;
    };

    class AmbientLight : public Light {
        public:
            AmbientLight(unsigned int color, float intensity);
            AmbientLight() = delete;

            ~AmbientLight() = default;
    };

    class PointLight : public Light {
        public:
            PointLight(const vec3& position, unsigned int color, const float intensity);
            PointLight() = delete;

            Object model_ = Object("src/light.obj", FILEFORMAT::OBJ, 0xFFFFFF);

            ~PointLight() = default;
    };
};