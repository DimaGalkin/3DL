#include "engine/lighting.hpp"
#include "engine/renderer.hpp"
#include "engine/camera.hpp"

int main(const int argc, const char** argv) {
    ThreeDL::Camera camera ({0, 0, 0}, {0, 0, 0}, 75);
    ThreeDL::Renderer renderer(camera, 1280, 720);

    ThreeDL::AmbientLight ambient (0xffffff, 0.1);
    ThreeDL::PointLight point ({0, 50, -500}, 0xffffff, 0.5);

    renderer.add(&ambient);
    renderer.add(&point);
    
    renderer.begin();

    return 0;
}