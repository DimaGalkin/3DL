#include "renderer.hpp"

ThreeDL::Renderer::Renderer(
        ThreeDL::Camera &camera,
        const uint32_t width,
        const uint32_t height
) : camera_(camera),
    width_(width),
    height_(height)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(width_, height_, 0, &window_, &renderer_);
    SDL_SetWindowTitle(window_, "3DL");

    pixels_surface_ = SDL_CreateRGBSurface(0, width_, height_, 32, 0, 0, 0, 0);

    zbuffer_.assign(width_ * height_, -INFINITY);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
    ImGui_ImplSDLRenderer2_Init(renderer_);

    state_ = State {
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

    render_program_ = cl::Kernel(render_program, "gpu_render");
    lighting_program_ = cl::Kernel(lighting_program, "gpu_lighting");

    renderLoop();

    SDL_Quit();
}

bool ThreeDL::Renderer::checkQuit() {
    SDL_Event event;
    SDL_PollEvent(&event);
    return event.type == SDL_QUIT;
}

void ThreeDL::Renderer::renderLoop() {
    SDL_Event event;
    bool client_quit = false;

    while (!client_quit) {
        //std::cout << "Rendering Frame..." << std::endl;

        client_quit |= checkQuit();

        pixels_buffer_ = cl::Buffer(
            ocl_utils_.context_,
            CL_MEM_WRITE_ONLY,
            sizeof(uint32_t) * width_ * height_
        );

        state_buffer_ = cl::Buffer(
            ocl_utils_.context_,
            CL_MEM_READ_ONLY,
            sizeof(State)
        );

        zbuffer_buffer_ = cl::Buffer(
            ocl_utils_.context_,
            CL_MEM_READ_WRITE,
            sizeof(double) * width_ * height_
        );

        lights_buffer_ = cl::Buffer(
            ocl_utils_.context_,
            CL_MEM_READ_ONLY,
            sizeof(GPULight) * lights_.size()
        );

        normal_buffer_ = cl::Buffer(
            ocl_utils_.context_,
            CL_MEM_READ_WRITE,
            sizeof(Vector3) * render_queue_.size()
        );

        position_buffer_ = cl::Buffer(
            ocl_utils_.context_,
            CL_MEM_READ_WRITE,
            sizeof(Vector3) * render_queue_.size()
        );

        diffuse_buffer_ = cl::Buffer(
            ocl_utils_.context_,
            CL_MEM_READ_WRITE,
            sizeof(uint32_t) * render_queue_.size()
        );

        specular_buffer_ = cl::Buffer(
            ocl_utils_.context_,
            CL_MEM_READ_WRITE,
            sizeof(uint32_t) * render_queue_.size()
        );

        client_quit |= checkQuit();

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

        client_quit |= checkQuit();

        state_.camera_position_ = camera_.position_.asGPUType();
        state_.camera_rotation_ = camera_.rotation_.asGPUType();

        for (const auto& object : render_queue_) {
            client_quit = client_quit || renderObject(*object);
        }

        client_quit |= checkQuit();

        for (const auto& light : lights_) {
            if (light->type_ != LightType::POINT) continue;
            client_quit = client_quit || renderObject(light->model_);
        }

        client_quit |= checkQuit();

        cl::NDRange global {width_ * height_};

        lighting_program_(
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

        client_quit |= checkQuit();

        gpu_lights_.clear();

        SDL_Surface* draw_surface = SDL_GetWindowSurface(window_);
        uint32_t* display = reinterpret_cast<uint32_t*>(draw_surface->pixels);

        SDL_LockSurface(draw_surface);

        ocl_utils_.queue_.enqueueReadBuffer(
            pixels_buffer_,
            CL_TRUE,
            0,
            sizeof(uint32_t) * width_ * height_,
            display
        );

        client_quit |= checkQuit();

        SDL_UnlockSurface(draw_surface);

        SDL_UpdateWindowSurface(window_);
    }
}

bool ThreeDL::Renderer::renderObject(const ThreeDL::Object& object) {
    bool client_quit = false;

    state_.texture_width_ = object.texture_w_;
    state_.texture_height_ = object.texture_h_;
    state_.model_position_ = object.position_.asGPUType();

    triangles_buffer_ = cl::Buffer(
        ocl_utils_.context_,
        CL_MEM_READ_ONLY,
        sizeof(Triangle) * object.triangles_.size()
    );

    texture_buffer_ = cl::Buffer(
        ocl_utils_.context_,
        CL_MEM_READ_ONLY,
        sizeof(uint32_t) * object.texture_.size()
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

    render_program_ (
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

    return client_quit;
}

ThreeDL::Renderer::~Renderer() {
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_FreeSurface(pixels_surface_);
}