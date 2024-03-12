#include "camera.hpp"

ThreeDL::Camera::Camera(const vec3& position, const vec3& rotation)
    :   position_(position),
        rotation_(rotation),
        fov_(75)
{}

ThreeDL::Camera::Camera(const ThreeDL::vec3& position, const ThreeDL::vec3& rotation, const double fov)
    :   position_(position),
        rotation_(rotation),
        fov_(fov)
{}

void ThreeDL::Camera::recalculateVectors() {
    forward_ = {0, 0, 1};
    right_ = {1, 0, 0};

    forward_.rotate(0, -rotation_.y, 0);
    right_.rotate(0, -rotation_.y, 0);

    forward_.normalize();
    right_.normalize();
}

void ThreeDL::Camera::moveForward(const double delta) {
    position_ += forward_ * delta;
}

void ThreeDL::Camera::moveRight(const double delta) {
    position_ += right_ * delta;
}

void ThreeDL::Camera::rotate(const ThreeDL::vec3& delta) {
    pan(delta.y);
    tilt(delta.x);
    roll(delta.z);
}

void ThreeDL::Camera::pan(const double delta) {
    rotation_.y += delta;
    rotation_.y = fmin(rotation_.y, max_rotation_.y);
}

void ThreeDL::Camera::tilt(const double delta) {
    rotation_.x += delta;
    rotation_.x = fmin(rotation_.x, max_rotation_.x);
}

void ThreeDL::Camera::roll(const double delta) {
    rotation_.z += delta;
    rotation_.z = fmin(rotation_.z, max_rotation_.z);
}