#include "lighting.hpp"

ThreeDL::Light::Light()
    : type_ {LightType::UNINITIALIZED},
      position_ {0, 0, 0},
      direction_ {0, 0, 0},
      intensity_ {0},
      name_("default"),
      color_ {0, 0, 0}
{}

GPULight ThreeDL::Light::asGPUType() const {
    GPULight light {
        .type_ = type_,
        .position_ = position_.asGPUType(),
        .direction_ = direction_.asGPUType(),
        .color_ = Utils::linearToUint(color_[0], color_[1], color_[2]),
        .intensity_ = intensity_
    };

    return light;
}

ThreeDL::AmbientLight::AmbientLight(std::string name, uint32_t color, float intensity) {
    name_ = name;
    type_ = LightType::AMBIENT;
    intensity_ = intensity;
    Utils::uintToLinear(color, color_);
}

ThreeDL::PointLight::PointLight(std::string name, const vec3& position, uint32_t color, const float intensity) {
    name_ = name;
    type_ = LightType::POINT;
    position_ = position;
    intensity_ = intensity;
    model_ =  Object("../src/light.obj", FILEFORMAT::OBJ, color);

    Utils::uintToLinear(color, color_);
}