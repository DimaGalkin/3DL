#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "gui.hpp"
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

    class Renderer {
        public:
            Renderer(Camera& camera, const uint32_t width, const uint32_t height);
            Renderer(const CameraController& controller, const uint32_t width, const uint32_t height);
            Renderer() = delete;

            void add(const Object* object);
            void add(const Light* light);

            void (*animation)(uint64_t ticks) = nullptr;

            void begin();

            ~Renderer();
        private:
            SDL_Renderer* renderer_;
            SDL_Window* window_;
            SDL_Surface* pixels_surface_;
            SDL_Texture* texture_;

            RENDERMODE mode_ = RENDERMODE::SHADED;

            State state_;

            uint32_t width_;
            uint32_t height_;

            std::mutex render_mutex_;
            Camera& camera_;

            GUI gui_;

            bool client_quit_ = false;

            bool frame_ready_ = false;

            bool gui_enabled_ = true;
            uint64_t enabled_ticks_ = 0;

            std::vector<double> zbuffer_;
            std::vector<const Object*> render_queue_;

            std::vector<GPULight> gpu_lights_;
            std::vector<const Light*> lights_;

            OpenCLUtils ocl_utils_;

            cl::Buffer zbuffer_buffer_;
            cl::Buffer pixels_buffer_;
            cl::Buffer state_buffer_;
            cl::Buffer triangles_buffer_;
            cl::Buffer texture_buffer_;
            cl::Buffer lights_buffer_;
            cl::Buffer normal_buffer_;
            cl::Buffer position_buffer_;
            cl::Buffer diffuse_buffer_;
            cl::Buffer specular_buffer_;

            void init();
            void render();
            void renderObject(const Object& object, gpu_render_program& program);
    };
};