#pragma once

#include <SDL.h>

#include <cmath>
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

#ifndef float3
typedef struct Vector3 float3;
#endif

#include "GPU/types.h"

namespace ThreeDL {
    class Utils {
        public:
            Utils() = delete;

            static std::vector<std::string> split(const std::string& str, const char delim);
            static uint32_t samplePixel(const SDL_Surface *surface, const uint32_t x, const uint32_t y);
            static uint32_t linearToUint(const float r, const float g, const float b);
            static void uintToLinear(const uint32_t color, float* colorArray);
            static float floatmin(const float a, const float b);

            ~Utils() = delete;
    };
};