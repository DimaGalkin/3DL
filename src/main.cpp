#include "engine/lighting.hpp"
#include "engine/renderer.hpp"
#include "engine/camera.hpp"

ThreeDL::CameraController controller (
        ThreeDL::vec3 {0, 0, 0},
        ThreeDL::vec3 {0, 0, 0},
        75
);

ThreeDL::Renderer renderer (controller, 1920, 1080);

ThreeDL::Object object (
        "../src/f15.obj",
        ThreeDL::FILEFORMAT::OBJ,
        "../src/f15.png"
);

ThreeDL::vec3 light_pos {object.centre_.x, object.centre_.y + 50, object.centre_.z};

ThreeDL::AmbientLight ambient (
        "ambient",
        0xffffff,
        0
);
ThreeDL::PointLight point (
        "point",
        light_pos,
        0xffffff,
        10000
);

int tick = 0;

void animation(uint64_t ticks) {
    controller.tick();

    ThreeDL::vec3 lp = ThreeDL::vec3 {
        100 * cos(tick / 1000.0),
        0,
        100 * sin(tick / 1000.0)
    };

    ++tick;

    point.position_ = light_pos + lp;
    point.model_.position_ = light_pos + lp;
}

int main(const int argc, const char** argv) {
    renderer.animation = animation;

    renderer.add(&ambient);
    renderer.add(&point);

    renderer.add(&object);
    //renderer.add(&floor_plane);
    
    renderer.begin();

    return 0;
}