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

#ifndef float3
typedef struct Vector3 float3;
#endif

#include "camera.hpp"
#include "GPU/types.h"
#include "objects.hpp"
#include "lighting.hpp"
#include "GPU/openclutils.hpp"

namespace ThreeDL {
    // TODO: Add comment with all the names of parameters in order
    using gpu_render_program = cl::compatibility::make_kernel<
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer,
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer,
            cl::Buffer, cl::Buffer, cl::Buffer
    >;
    using gpu_lighting_program = cl::compatibility::make_kernel<
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer,
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer
    >;
    using gpu_shadow_program = cl::compatibility::make_kernel<
            cl::Buffer, cl::Buffer, cl::Buffer
    >;
    using gpu_fragment_program = cl::compatibility::make_kernel<
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer,
            cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer
    >;

    /**
     * @class Renderer
     * @brief Encapsulates a 3D scene, provides debugging tools (GUI) and uses OpenCL to render the scene.
     */
    class Renderer {
        public:
            /**
             * @method Renderer Constructor
             * @brief This constructor takes a reference to a camera, not const as renderer may adjust internal values.
            */
            Renderer(Camera& camera, uint32_t width, uint32_t height);

            /**
              * @method Renderer Constructor
              * @brief This constructor takes a reference to a camera controller, this will extract the camera reference from the controller.
            */
            Renderer(CameraController& controller, uint32_t width, uint32_t height);

            // Need camera and size info
            Renderer() = delete;

            /**
             * @method add
             * @brief adds an Object (triangles & texture) or a light to the render pipeline.
            */
            void add(const Object* object);
            void add(Light* light);

            /**
             * @method animation
             * @brief user defined method called every millisecond, can be used to create moving objects.
            */
            void (*animation)(uint64_t ticks) = nullptr;

            /**
             * @method begin
             * @brief Creates SDL window and stats scanning inputs and launches render thread which makes calls to GPU.
            */
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
            CameraController* controller_;

            bool camera_controller_ = false;
            bool anim_enabled_ = true;
            bool client_quit_ = false;
            bool frame_ready_ = false;
            bool gui_enabled_ = true;
            uint64_t enabled_ticks_ = 0;

            Light* selected_light_ = nullptr;

            std::vector<float> zbuffer_;
            std::vector<const Object*> render_queue_;

            std::vector<GPULight> gpu_lights_;
            std::vector<Light*> lights_;

            OpenCLUtils ocl_utils_;

            cl::Buffer zbuffer_buffer_;
            cl::Buffer t_buffer_;
            cl::Buffer tri_store_buffer_;
            cl::Buffer pixels_buffer_;
            cl::Buffer state_buffer_;
            cl::Buffer triangles_buffer_;
            cl::Buffer texture_buffer_;
            cl::Buffer lights_buffer_;
            cl::Buffer normal_buffer_;
            cl::Buffer position_buffer_;
            cl::Buffer diffuse_buffer_;
            cl::Buffer specular_buffer_;
            cl::Buffer color_buffer_;

            void init();
            void renderGUIWindows(uint64_t fps);
            void render();
            void renderObject(const Object& object, gpu_render_program& program);
    };
};