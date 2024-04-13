#include "engine/lighting.hpp"
#include "engine/renderer.hpp"
#include "engine/camera.hpp"

ThreeDL::CameraController controller (
        ThreeDL::vec3 {0, 0, 0},
        ThreeDL::vec3 {0, 0, 0},
        75
);

ThreeDL::Renderer renderer (controller, 1920, 1080);

ThreeDL::vec3 light_pos {-135, 50, -195};

ThreeDL::AmbientLight ambient (0xffffff, 0);
ThreeDL::PointLight point (light_pos, 0xffffff, 10000);

ThreeDL::Object object (
        "../src/f4.obj",
        ThreeDL::FILEFORMAT::OBJ,
        "../src/f4.png"
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