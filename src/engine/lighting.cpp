#include "lighting.hpp"

ThreeDL::Light::Light()
    : type_ {LightType::UNINITIALIZED},
      position_ {0, 0, 0},
      direction_ {0, 0, 0},
      intensity_ {0},
      name_ {"default"},
      color_ {0, 0, 0},
      fov_ { 75 }
{}

GPULight ThreeDL::Light::asGPUType() const {
    return GPULight {
        .type_ = type_,
        .position_ = position_.asGPUType(),
        .direction_ = direction_.asGPUType(),
        .shadows_enabled_ = shadows_enabled_,
        .shadow_map_width_ = shadowmap_w_,
        .shadow_map_height_ = shadowmap_h_,
        .fov_ = fov_,
        .color_ = Utils::linearToUint(color_[0], color_[1], color_[2]),
        .intensity_ = intensity_
    };
}

ThreeDL::AmbientLight::AmbientLight(const std::string& name, const uint32_t color, const float intensity) {
    name_ = name;
    type_ = LightType::AMBIENT;
    intensity_ = intensity;

    Utils::uintToLinear(color, color_);
}

ThreeDL::PointLight::PointLight(const std::string& name, const vec3& position, const uint32_t color, const float intensity) {
    name_ = name;
    type_ = LightType::POINT;
    position_ = position;
    intensity_ = intensity;
    model_ =  Object("../light.obj", FILEFORMAT::OBJ, color);

    Utils::uintToLinear(color, color_);
}

ThreeDL::DirectionalLight::DirectionalLight(const std::string& name, const vec3& direction, const vec3& position, const float fov, const uint32_t color, const float intensity) {
    name_ = name;
    type_ = LightType::DIRECTIONAL;
    direction_ = direction;
    position_ = position;
    intensity_ = intensity;
    fov_ = fov;
    model_ = Object("../light.obj", FILEFORMAT::OBJ, color);

    Utils::uintToLinear(color, color_);
}