#pragma once

#ifndef float3
typedef struct Vector3 float3;
#endif

#include "GPU/types.h"

// cpp & hpp included so linking templates doesn't fail
#include "linalg.hpp"
#include "linalg.cpp"

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
            float fov_;
            bool shadows_enabled_ = true;
            uint32_t shadowmap_w_ = 1024;
            uint32_t shadowmap_h_ = 1024;

            ~Light() = default;
    };

    class AmbientLight : public Light {
        public:
            AmbientLight(const std::string& name, uint32_t color, float intensity);
            AmbientLight() = delete;

            ~AmbientLight() = default;
    };

    class PointLight : public Light {
        public:
            PointLight(const std::string& name, const vec3& position, uint32_t color, const float intensity);
            PointLight() = delete;

            ~PointLight() = default;
    };

    class DirectionalLight : public Light {
        public:
            DirectionalLight(const std::string& name, const vec3& direction, const vec3& position, const float fov, uint32_t color, const float intensity);
            DirectionalLight() = delete;

            ~DirectionalLight() = default;
    };
};