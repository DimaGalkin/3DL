#pragma once

#define _USE_MATH_DEFINES // for intellisense
#include <math.h>
#include <stdint.h>
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