#include "utils.hpp"

std::vector<std::string> ThreeDL::Utils::split(const std::string &str, const char delim) {
    std::vector<std::string> result;
    std::stringstream ss (str);
    std::string item;

    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

uint32_t ThreeDL::Utils::samplePixel(const SDL_Surface *surface, const uint32_t x, const uint32_t y) {
    const uint8_t bpp = surface->format->BytesPerPixel;
    uint8_t *p = reinterpret_cast<uint8_t*>(surface->pixels) + y * surface->pitch + x * bpp;

    return *reinterpret_cast<uint32_t*>(p);
}

uint32_t ThreeDL::Utils::linearToUint(const double r, const double g, const double b) {
    return (static_cast<uint8_t>(r * 255) << 16) | (static_cast<uint8_t>(g * 255) << 8) | static_cast<uint8_t>(b * 255);
}

void ThreeDL::Utils::uintToLinear(const uint32_t color, float* colorArray) {
    colorArray[0] = static_cast<float>((color >> 16) & 0xFF) / 255.0f;
    colorArray[1] = static_cast<float>((color >> 8) & 0xFF) / 255.0f;
    colorArray[2] = static_cast<float>(color & 0xFF) / 255.0f;
}