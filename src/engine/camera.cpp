#include "camera.hpp"

ThreeDL::Camera::Camera(const vec3& position, const vec3& rotation)
    :   position_(position),
        rotation_(rotation),
        fov_(75)
{
    recalculateVectors();
}

ThreeDL::Camera::Camera(const ThreeDL::vec3& position, const ThreeDL::vec3& rotation, const float fov)
    :   position_(position),
        rotation_(rotation),
        fov_(fov)
{
    recalculateVectors();
}

void ThreeDL::Camera::recalculateVectors() {
    forward_ = {0, 0, 1};
    right_ = {1, 0, 0};

    forward_.rotate(0, -rotation_.y, 0);
    right_.rotate(0, -rotation_.y, 0);

    forward_.normalize();
    right_.normalize();
}

void ThreeDL::Camera::moveForward(const float delta) {
    position_ += forward_ * delta;
}

void ThreeDL::Camera::moveRight(const float delta) {
    position_ += right_ * delta;
}

void ThreeDL::Camera::rotate(const ThreeDL::vec3& delta) {
    pan(delta.y);
    tilt(delta.x);
    roll(delta.z);
}

void ThreeDL::Camera::pan(const float delta) {
    rotation_.y += delta;
    rotation_.y = fmin(rotation_.y, max_rotation_.y);
}

void ThreeDL::Camera::tilt(const float delta) {
    rotation_.x += delta;
    rotation_.x = fmin(rotation_.x, max_rotation_.x);
}

void ThreeDL::Camera::roll(const float delta) {
    rotation_.z += delta;
    rotation_.z = fmin(rotation_.z, max_rotation_.z);
}

ThreeDL::CameraController::CameraController(Camera& camera)
        : camera_ {camera},
          internal_camera_ {zero_vec3, zero_vec3, 0} // populate with dummy values
{}

ThreeDL::CameraController::CameraController(const vec3& position, const vec3& rotation, const float fov)
        : camera_ {internal_camera_},
          internal_camera_ {position, rotation, fov}
{}

void ThreeDL::CameraController::tick() {
    const uint8_t* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_W]) {
        camera_.moveForward(-0.1);
    }

    if (keys[SDL_SCANCODE_S]) {
        camera_.moveForward(0.1);
    }

    if (keys[SDL_SCANCODE_A]) {
        camera_.moveRight(0.1);
    }

    if (keys[SDL_SCANCODE_D]) {
        camera_.moveRight(-0.1);
    }

    if (keys[SDL_SCANCODE_UP]) {
        camera_.rotate({0.1, 0, 0});
    }

    if (keys[SDL_SCANCODE_DOWN]) {
        camera_.rotate({-0.1, 0, 0});
    }

    if (keys[SDL_SCANCODE_LEFT]) {
        camera_.rotate({0, 0.1, 0});
    }

    if (keys[SDL_SCANCODE_RIGHT]) {
        camera_.rotate({0, -0.1, 0});
    }

    camera_.recalculateVectors();
}

void ThreeDL::CameraController::translate(const vec3& delta) {
    camera_.position_ += delta;
}

void ThreeDL::CameraController::rotate(const vec3& delta) {
    camera_.rotate(delta);
}