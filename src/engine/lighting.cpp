#include "lighting.hpp"

ThreeDL::Light::Light() {
    position_ = vec3 {0, 0, 0};
    direction_ = vec3 {0, 0, 0};
    intensity_ = 0.0f;
}

GPULight ThreeDL::Light::asGPUType() const {
    GPULight light;

    light.position_ = position_.asGPUType();
    light.direction_ = direction_.asGPUType();
    light.intensity_ = intensity_;
    light.color_ = Utils::linearToUint(color_[0], color_[1], color_[2]);
    light.type_ = type_;

    return light;
}

ThreeDL::AmbientLight::AmbientLight(unsigned int color, float intensity) {
    type_ = LightType::AMBIENT;
    intensity_ = intensity;
    Utils::uintToLinear(color, color_);
}

ThreeDL::PointLight::PointLight(const vec3& position, unsigned int color, const float intensity) {
    type_ = LightType::POINT;
    position_ = position;
    intensity_ = intensity;
    Utils::uintToLinear(color, color_);
}