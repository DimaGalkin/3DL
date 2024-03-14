#include "engine/lighting.hpp"
#include "engine/renderer.hpp"
#include "engine/camera.hpp"

int main(const int argc, const char** argv) {
    ThreeDL::Camera camera ({0, 0, 0}, {0, 0, 0}, 36);
    ThreeDL::Renderer renderer(camera, 1280, 720);

    ThreeDL::AmbientLight ambient (0xffffff, 0.1);
    ThreeDL::PointLight point ({0, 50, -500}, 0xffffff, 10000);

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