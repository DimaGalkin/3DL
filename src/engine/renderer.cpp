#include "renderer.hpp"

ThreeDL::Renderer::Renderer(
        ThreeDL::Camera &camera,
        const uint32_t width,
        const uint32_t height
) : width_(width),
    height_(height),
    camera_(camera),
    gui_(nullptr, nullptr)
{
    init();
}

ThreeDL::Renderer::Renderer(
        const ThreeDL::CameraController& controller,
        const uint32_t width,
        const uint32_t height
) : width_(width),
    height_(height),
    camera_(controller.getCamera()),
    gui_(nullptr, nullptr)
{
    init();
}

void ThreeDL::Renderer::init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(width_, height_, 0, &window_, &renderer_);
    SDL_SetWindowTitle(window_, "3DL");

    zbuffer_.assign(width_ * height_, -INFINITY);

    state_ = State {
            mode_,
            camera_.position_.asGPUType(),
            camera_.rotation_.asGPUType(),
            camera_.fov_,
            {0, 0, 0},
            width_,
            height_,
            0,
            0,
            0
    };
}

void ThreeDL::Renderer::add(const ThreeDL::Object *object) {
    render_queue_.push_back(object);
}

void ThreeDL::Renderer::add(const ThreeDL::Light *light) {
    lights_.push_back(light);
}

void ThreeDL::Renderer::begin() {
    std::thread render_thread (&ThreeDL::Renderer::render, this);

    uint64_t ticks_at_last_frame = 0;

    SDL_Event event;

    IMGUI_CHECKVERSION();
    ImGuiContext* cont = ImGui::CreateContext();
    ImGui::SetCurrentContext(cont);
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
    ImGui_ImplSDLRenderer2_Init(renderer_);

    while (!client_quit_) {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) {
            client_quit_ = true;
        }

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_F12 && SDL_GetTicks64() - enabled_ticks_ > 5) {
                 gui_enabled_ = !gui_enabled_;
            } else {
                enabled_ticks_ = SDL_GetTicks64();
            }
        }

        ImGui_ImplSDL2_ProcessEvent(&event);

        if (frame_ready_ && gui_enabled_) {
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            texture_ = SDL_CreateTextureFromSurface(renderer_, pixels_surface_);
            SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
            SDL_RenderCopy(renderer_, texture_, NULL, NULL);
            SDL_DestroyTexture(texture_);

            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

            SDL_RenderPresent(renderer_);
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderClear(renderer_);

            frame_ready_ = false;
        }

        uint64_t delta_ticks = SDL_GetTicks64() - ticks_at_last_frame;

        if (delta_ticks == 0) continue;

        render_mutex_.lock();
        animation(delta_ticks);
        render_mutex_.unlock();

        ticks_at_last_frame = SDL_GetTicks64();
    }

    render_thread.join();
}

void ThreeDL::Renderer::render() {
    pixels_surface_ = SDL_CreateRGBSurface(0, width_, height_, 32, 0, 0, 0, 0);

    std::cout << "Starting Kernel Compilation..." << std::endl;

    cl::Program render_program = ocl_utils_.buildProgram(
        "gpu_render",
        "src/engine/GPU/kernel.cl"
    );
    cl::Program lighting_program = ocl_utils_.buildProgram(
        "gpu_lighting",
        "src/engine/GPU/kernel.cl"
    );

    std::cout << "Kernel Compilation Complete!" << std::endl;

    gpu_render_program gpu_render (cl::Kernel(render_program, "gpu_render"));
    gpu_lighting_program gpu_lighting (cl::Kernel(lighting_program, "gpu_lighting"));

    while (!client_quit_) {
        bool show_gui = gui_enabled_;

        pixels_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(uint32_t) * width_ * height_
        );

        state_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(State)
        );

        zbuffer_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(double) * width_ * height_
        );

        lights_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(GPULight) * lights_.size()
        );

        normal_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(Vector3) * width_ * height_
        );

        position_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(Vector3) * width_ * height_
        );

        diffuse_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(uint32_t) * width_ * height_
        );

        specular_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(uint32_t) * width_ * height_
        );

        for (const auto& light : lights_) {
            gpu_lights_.push_back(light->asGPUType());
        }

        ocl_utils_.queue_.enqueueWriteBuffer(
                lights_buffer_,
                CL_TRUE,
                0,
                sizeof(GPULight) * lights_.size(),
                gpu_lights_.data()
        );

        ocl_utils_.queue_.enqueueWriteBuffer(
                zbuffer_buffer_,
                CL_TRUE,
                0,
                sizeof(double) * width_ * height_,
                zbuffer_.data()
        );

        render_mutex_.lock();

        state_.camera_position_ = camera_.position_.asGPUType();
        state_.camera_rotation_ = camera_.rotation_.asGPUType();
        state_.fov_ = camera_.fov_;

        state_.width_ = width_;
        state_.height_ = height_;

        state_.num_lights_ = lights_.size();
        state_.mode_ = mode_;

        render_mutex_.unlock();

        for (const auto& object : render_queue_) {
            renderObject(*object, gpu_render);
        }

        for (const auto& light : lights_) {
            if (light->type_ != LightType::POINT) continue;

            render_mutex_.lock();

            Object model = light->model_;
            model.position_ = light->position_;

            render_mutex_.unlock();

            renderObject(model, gpu_render);
        }

        cl::NDRange global {width_ * height_};

        gpu_lighting (
            cl::EnqueueArgs(ocl_utils_.queue_, global),
            pixels_buffer_,
            lights_buffer_,
            state_buffer_,
            diffuse_buffer_,
            specular_buffer_,
            normal_buffer_,
            position_buffer_,
            zbuffer_buffer_
        ).wait();

        gpu_lights_.clear();

        SDL_Surface* draw_surface;

        if (show_gui) {
            draw_surface = pixels_surface_;
            frame_ready_ = false;
        } else {
            draw_surface = SDL_GetWindowSurface(window_);
        }

        auto* display = reinterpret_cast<uint32_t*>(draw_surface->pixels);

        SDL_LockSurface(draw_surface);
        ocl_utils_.queue_.enqueueReadBuffer(
                pixels_buffer_,
                CL_TRUE,
                0,
                sizeof(uint32_t) * width_ * height_,
                display
        );
        SDL_UnlockSurface(draw_surface);

        if (show_gui) {
            frame_ready_ = true;
        } else {
            SDL_UpdateWindowSurface(window_);
        }
    }
}

void ThreeDL::Renderer::renderObject(const ThreeDL::Object& object, gpu_render_program& gpu_render) {
    render_mutex_.lock();

    state_.texture_width_ = object.texture_w_;
    state_.texture_height_ = object.texture_h_;
    state_.model_position_ = object.position_.asGPUType();

    render_mutex_.unlock();

    triangles_buffer_ = cl::Buffer(
        ocl_utils_.context_,
        CL_MEM_READ_WRITE,
        sizeof(Triangle) * object.triangles_.size()
    );

    texture_buffer_ = cl::Buffer(
        ocl_utils_.context_,
        CL_MEM_READ_WRITE,
        sizeof(uint32_t) * object.texture_w_ * object.texture_h_
    );

    ocl_utils_.queue_.enqueueWriteBuffer(
        triangles_buffer_,
        CL_TRUE,
        0,
        sizeof(Triangle) * object.triangles_.size(),
        object.triangles_.data()
    );

    ocl_utils_.queue_.enqueueWriteBuffer(
        texture_buffer_,
        CL_TRUE,
        0,
        sizeof(uint32_t) * object.texture_.size(),
        object.texture_.data()
    );

    ocl_utils_.queue_.enqueueWriteBuffer(
        state_buffer_,
        CL_TRUE,
        0,
        sizeof(State),
        &state_
    );

    cl::NDRange global {object.triangles_.size()};

    gpu_render (
        cl::EnqueueArgs(ocl_utils_.queue_, global),
        triangles_buffer_,
        lights_buffer_,
        state_buffer_,
        zbuffer_buffer_,
        texture_buffer_,
        normal_buffer_,
        position_buffer_,
        diffuse_buffer_,
        specular_buffer_,
        pixels_buffer_
    ).wait();
}

ThreeDL::Renderer::~Renderer() {
    SDL_Quit();

//    ImGui_ImplSDL2_Shutdown();
//    ImGui_ImplSDLRenderer2_Shutdown();
//    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_FreeSurface(pixels_surface_);
}