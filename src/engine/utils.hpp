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
    class Utils {
        public:
            Utils() = delete;

            static std::vector<std::string> split(const std::string& str, char delim);
            static uint32_t samplePixel(const SDL_Surface *surface, const uint32_t x, const uint32_t y);
            static uint32_t linearToUint(const double r, const double g, const double b);
            static void uintToLinear(const uint32_t color, float* colorArray);

            ~Utils() = delete;
    };
};