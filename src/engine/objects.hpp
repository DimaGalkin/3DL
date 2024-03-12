#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <fstream>
#include <string>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include "GPU/types.h"
#include "linalg.hpp"

namespace ThreeDL {
    using tri_allocator_t = cl::SVMAllocator<Triangle, cl::SVMTraitCoarse<>>;
    using tex_allocator_t = cl::SVMAllocator<uint32_t, cl::SVMTraitCoarse<>>;

    using tribuf_t = std::vector<Triangle, tri_allocator_t>;
    using texbuf_t = std::vector<uint32_t, tex_allocator_t>;

    enum class FILEFORMAT {
        OBJ
    };

    class Object {
        public:
            Object(const std::string& model_path, const FILEFORMAT format, const std::string& texture_path);
            Object(const std::string& model_path, const FILEFORMAT ff, const uint32_t color);
            Object() = default;

            vec3 position_;
            vec3 rotation_;

            uint32_t texture_w_;
            uint32_t texture_h_;

            tribuf_t triangles_;
            texbuf_t texture_;

            ~Object() = default;
        private:
            void fillTextureBuffer(const std::string& texture_path);
            void fillTextureBuffer(const uint32_t color);
            void fillTriangleBuffer(const std::string& model_path, const FILEFORMAT format);
    };

    class OBJLoader {
        public:
            OBJLoader() = delete;

            // add support for mtl files

            static std::vector<Triangle> load(const std::string& path);

            ~OBJLoader() = delete;
    };
};