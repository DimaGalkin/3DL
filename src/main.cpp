#include "engine/lighting.hpp"
#include "engine/renderer.hpp"
#include "engine/controller.hpp"

int main(const int argc, const char** argv) {
    ThreeDL::CameraController controller (
        ThreeDL::vec3 {0, 0, 0},
        ThreeDL::vec3 {0, 0, 0},
        36
    );

    ThreeDL::Renderer renderer(controller, 1280, 720);

    ThreeDL::AmbientLight ambient (0xffffff, 0.1);
    ThreeDL::PointLight point ({0, 50, -500}, 0xffffff, 1000);

    ThreeDL::Object object (
        "src/f15.obj",
        ThreeDL::FILEFORMAT::OBJ,
        "src/f15.png"
    );

    renderer.add(&ambient);
    renderer.add(&point);
    renderer.add(&object);
    
    renderer.begin();

    return 0;
}