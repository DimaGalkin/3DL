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
            bool model_enabled_ = true;

            std::string name_;

            LightType type_;
            vec3 position_;
            vec3 direction_;
            float intensity_;
            float color_[3];

            ~Light() = default;
    };

    class AmbientLight : public Light {
        public:
            AmbientLight(std::string name, uint32_t color, float intensity);
            AmbientLight() = delete;

            ~AmbientLight() = default;
    };

    class PointLight : public Light {
        public:
            PointLight(std::string name, const vec3& position, uint32_t color, const float intensity);
            PointLight() = delete;

            ~PointLight() = default;
    };
};