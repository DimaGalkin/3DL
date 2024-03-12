#pragma once

#include <SDL.h>

#define _USE_MATH_DEFINES // for intellisense
#include <math.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include "GPU/types.h"

namespace ThreeDL {
    class LinearColData {
        public:
            LinearColData() = default;

            float r;
            float g;
            float b;

            ~LinearColData() = default;
    };

    class Utils {
        public:
            Utils() = delete;

            static std::vector<std::string> split(const std::string& str, char delim);
            static uint32_t samplePixel(const SDL_Surface *surface, const uint32_t x, const uint32_t y);

            ~Utils() = delete;
    };

    class LinearCol {
        public:
            LinearCol() = delete;

            static LinearColData RGBtoLinear(const uint32_t color) {
                unsigned int r = (color & 0xFF0000) >> 16;
                unsigned int g = (color & 0x00FF00) >> 8;
                unsigned int b = (color & 0x0000FF);

                float lin_r = static_cast<float>(r) / 255.0f;
                float lin_g = static_cast<float>(g) / 255.0f;
                float lin_b = static_cast<float>(b) / 255.0f;

                return {lin_r, lin_g, lin_b};
            }

            ~LinearCol() = delete;
    } typedef linearcol;
};