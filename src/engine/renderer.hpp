#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <vector>

#include "camera.hpp"
#include "GPU/types.h"
#include "objects.hpp"
#include "lighting.hpp"
#include "GPU/openclutils.hpp"

namespace ThreeDL {
    using gpu_render_program = cl::compatibility::make_kernel<
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer,
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer
    >;
    using gpu_lighting_program = cl::compatibility::make_kernel<
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer,
            cl::Buffer, cl::Buffer, cl::Buffer
    >;

    enum class RENDERMODE {
        WIREFRAME,
        WIREFRAME_OVERLAY,
        SHADED
    };

    class Renderer {
        public:
            Renderer(const Camera& camera, const uint32_t width, const uint32_t height);
            Renderer() = delete;

            void add(const Object* object);
            void add(const Light* light);

            void (*animation)(double delta_time) = nullptr;

            void begin();

            ~Renderer();
        private:
            const SDL_Renderer* renderer_;
            const SDL_Window* window_;
            const SDL_Surface* pixels_surface_;
            SDL_Texture* pixels_texture_;

            Camera& camera_;

            State state_;

            uint32_t width_;
            uint32_t height_;

            bool gui_enabled_ = false;
            uint64_t enabled_ticks_ = 0;

            std::vector<double> zbuffer_;
            std::vector<Object*> render_queue_;

            std::vector<GPULight> gpu_lights_;
            std::vector<Light*> lights_;

            OpenCLUtils ocl_utils_;
            gpu_render_program render_program_;
            gpu_lighting_program lighting_program_;

            cl::Buffer zbuffer_buffer_;
            cl::Buffer pixels_buffer_;
            cl::Buffer scene_info_buffer_;
            cl::Buffer triangles_buffer_;
            cl::Buffer texture_buffer_;
            cl::Buffer lights_buffer_;
            cl::Buffer normal_buffer_;
            cl::Buffer position_buffer_;
            cl::Buffer diffuse_buffer_;
            cl::Buffer specular_buffer_;

            void renderObject(const Object& object);
    };
};