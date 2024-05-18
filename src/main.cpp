#include "engine/lighting.hpp"
#include "engine/renderer.hpp"
#include "engine/camera.hpp"

ThreeDL::CameraController controller (
        ThreeDL::vec3 {0, 0, 0},
        ThreeDL::vec3 {0, 90, 0},
        75
);
ThreeDL::Renderer renderer (controller, 1920, 1080);

ThreeDL::Object plane (
        "src/MiG-29.obj",
        ThreeDL::FILEFORMAT::OBJ,
        "src/MIG-29.png"
);

ThreeDL::vec3 light_pos1 {plane.centre_.x, plane.centre_.y + 50, plane.centre_.z};

ThreeDL::AmbientLight ambient (
        "ambient",
        0xffffff,
        0.2
);

ThreeDL::DirectionalLight point (
        "point",
        ThreeDL::vec3 {0, 1, 0},
        light_pos1,
        60,
        0xffffff,
        10000
);


int tick = 0;
void animation(uint64_t ticks) {
    controller.tick();

    ThreeDL::vec3 lp = ThreeDL::vec3 {
        50 * (float)cos(tick / 1000.0),
        0,
        50 * (float)sin(tick / 1000.0)
    };

    ++tick;

    point.position_ = light_pos1 + lp;
    point.getModelPosition() = light_pos1 + lp;
}

int main(const int argc, const char** argv) {
        renderer.animation = animation;
        renderer.add(&ambient);

        renderer.add(&point);

        renderer.add(&plane);
        renderer.begin();

        return 0;
}