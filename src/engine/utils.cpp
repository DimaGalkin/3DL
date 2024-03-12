#include "utils.hpp"

std::vector<std::string> ThreeDL::Utils::split(const std::string &str, char delim) {
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