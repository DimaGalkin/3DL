#include "controller.hpp"

ThreeDL::CameraController::CameraController(Camera& camera)
    :   camera_ {camera},
        internal_camera_ {zero_vec3, zero_vec3, 0} // populate with dummy values
{}

ThreeDL::CameraController::CameraController(const vec3& position, const vec3& rotation, const float fov)
    : camera_ {internal_camera_},
      internal_camera_ {position, rotation, fov}
{}

void ThreeDL::CameraController::tick(double delta_time) {
    SDL_Event event;
    SDL_PollEvent(&event);

    double speed = 0.1 * delta_time;

    switch (event.key.keysym.sym) {
        case SDLK_w:
            internal_camera_.moveForward(speed);
            break;
        case SDLK_s:
            internal_camera_.moveForward(-speed);
            break;
        case SDLK_a:
            internal_camera_.moveRight(-speed);
            break;
        case SDLK_d:
            internal_camera_.moveRight(speed);
            break;
        case SDLK_q:
            internal_camera_.rotate({0, 0, speed});
            break;
        case SDLK_e:
            internal_camera_.rotate({0, 0, -speed});
            break;
        case SDLK_UP:
            internal_camera_.rotate({0, speed, 0});
            break;
        case SDLK_DOWN:
            internal_camera_.rotate({0, -speed, 0});
            break;
        case SDLK_LEFT:
            internal_camera_.rotate({-speed, 0, 0});
            break;
        case SDLK_RIGHT:
            internal_camera_.rotate({speed, 0, 0});
            break;
    }
}

void ThreeDL::CameraController::translate(const vec3& delta) {
    internal_camera_.position_ += delta;
}

void ThreeDL::CameraController::rotate(const vec3& delta) {
    internal_camera_.rotate(delta);
}