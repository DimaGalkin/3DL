#include "renderer.hpp"

ThreeDL::Renderer::Renderer(
        ThreeDL::Camera &camera,
        const uint32_t width,
        const uint32_t height
) : width_(width),
    height_(height),
    camera_(camera),
    controller_(nullptr)
{
    init();
}

ThreeDL::Renderer::Renderer(
        ThreeDL::CameraController& controller,
        const uint32_t width,
        const uint32_t height
) : width_(width),
    height_(height),
    camera_(controller.getCamera()),
    controller_(&controller),
    camera_controller_(true)
{
    init();
    SDL_SetRelativeMouseMode(SDL_FALSE);
    controller_->screen_centre_.x = static_cast<int>(width_/2);
    controller_->screen_centre_.y = static_cast<int>(height_/2);
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

void ThreeDL::Renderer::add(ThreeDL::Light *light) {
    lights_.push_back(light);
}

void ThreeDL::Renderer::renderGUIWindows(uint64_t fps) {
    int full_height = height_ - 19;
    int renderer_window_height = (full_height / 5) * 1;
    int camera_window_height = (full_height / 5) * 1.25;
    int lights_window_height = (full_height / 5) * 2;
    int animation_window_height = (full_height / 5) * 0.75;

    int renderer_window_y = 19;
    int camera_window_y = renderer_window_y + renderer_window_height;
    int lights_window_y = camera_window_y + camera_window_height;
    int animation_window_y = lights_window_y + lights_window_height;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Lights"))
        {
            if (lights_.empty()) {
                ImGui::Text("No Lights available!");
            } else {
                for (auto &light: lights_) {
                    if (ImGui::MenuItem(light->getName().c_str())) {
                        selected_light_ = light;
                    }
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::SetNextWindowPos(ImVec2(0, renderer_window_y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, renderer_window_height));

    ImGui::Begin("Renderer", nullptr,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("FPS: %ld", fps);

    ImGui::Text("Renderer Resolution: %dx%d", width_, height_);
    ImGui::Text("Renderer Mode: %s", mode_ == RENDERMODE::SHADED ? "Shaded" : "Wireframe");

    if (ImGui::Button("Toggle Mode")) {
        mode_ = mode_ == RENDERMODE::SHADED ? RENDERMODE::WIREFRAME : RENDERMODE::SHADED;
    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0, camera_window_y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, camera_window_height));

    ImGui::Begin("Camera", nullptr,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Camera Position:");
    ImGui::Text("   X: %f", camera_.position_.x);
    ImGui::Text("   Y: %f", camera_.position_.y);
    ImGui::Text("   Z: %f", camera_.position_.z);
    ImGui::Text("\n");

    ImGui::Text("Camera Rotation:");
    ImGui::Text("   X: %f", camera_.rotation_.x);
    ImGui::Text("   Y: %f", camera_.rotation_.y);
    ImGui::Text("   Z: %f", camera_.rotation_.z);
    ImGui::Text("\n");

    ImGui::Text("Camera FOV: %f", camera_.fov_);
    //fov slider
    ImGui::SliderFloat("FOV", &camera_.fov_, 0, 180);

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0, animation_window_y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, animation_window_height));

    ImGui::Begin("Animation", nullptr,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Animation Callback: %s", animation == nullptr ? "None" : "Set");

    if (ImGui::Button("Toggle Animation")) {
        anim_enabled_ = !anim_enabled_;
    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0, lights_window_y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, lights_window_height));

    ImGui::Begin("Light", nullptr,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (lights_.empty()) {
        ImGui::Text("Nothing to show here!");
    } else if (selected_light_ != nullptr) {
        ImGui::Text("Selected Light: %s", selected_light_->getName().c_str());

        if (selected_light_->getType() == LightType::POINT) {
            ImGui::Text("Light Type: Point");
        } else {
            ImGui::Text("Light Type: Ambient");
        }
        ImGui::Text("\n");

        ImGui::Text("Light Position:");
        ImGui::Text("   X: %f", selected_light_->position_.x);
        ImGui::Text("   Y: %f", selected_light_->position_.y);
        ImGui::Text("   Z: %f", selected_light_->position_.z);
        ImGui::Text("\n");

        ImGui::ColorPicker3("Light Color", selected_light_->color_);
        ImGui::Text("\n");

        ImGui::Text("Light Intensity: %f", selected_light_->intensity_);

        if (selected_light_->getType() == LightType::AMBIENT) {
            ImGui::SliderFloat("Intensity", &selected_light_->intensity_, 0, 1);
        } else {
            ImGui::SliderFloat("Intensity", &selected_light_->intensity_, 0, 100000);
            ImGui::SliderFloat("FOV", &selected_light_->fov_, 0, 180);
        }
        ImGui::Text("\n");

        if (ImGui::Button("Toggle Model Visibility")) {
            selected_light_->model_enabled_ = !selected_light_->model_enabled_;
        }

        if (ImGui::Button("Delete Light")) {
            lights_.erase(std::remove(lights_.begin(), lights_.end(), selected_light_), lights_.end());
            selected_light_ = nullptr;
        }
    } else {
        selected_light_ = lights_[0];
    }

    ImGui::End();
}

void ThreeDL::Renderer::begin() {
    std::thread render_thread (&ThreeDL::Renderer::render, this);

    uint64_t ticks_at_last_frame = 0;
    uint64_t ticks_at_last_anim = 0;

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
            if (event.key.keysym.sym == SDLK_F12 && SDL_GetTicks64() - enabled_ticks_ > 10) {
                 gui_enabled_ = !gui_enabled_;
                 controller_->rotation_enabled_ = !controller_->rotation_enabled_;
                 SDL_SetRelativeMouseMode(!gui_enabled_ ? SDL_TRUE : SDL_FALSE);
                 SDL_WarpMouseInWindow(window_, width_/2, height_/2);
            } else {
                enabled_ticks_ = SDL_GetTicks64();
            }
        }

        ImGui_ImplSDL2_ProcessEvent(&event);

        uint64_t delta_ticks = SDL_GetTicks64() - ticks_at_last_frame;
        uint64_t animation_ticks = SDL_GetTicks64() - ticks_at_last_anim;

        if (frame_ready_ && gui_enabled_) {

            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            renderGUIWindows(1000 / delta_ticks);

            texture_ = SDL_CreateTextureFromSurface(renderer_, pixels_surface_);
            SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
            SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
            SDL_DestroyTexture(texture_);

            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

            SDL_RenderPresent(renderer_);
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderClear(renderer_);

            frame_ready_ = false;
            ticks_at_last_frame = SDL_GetTicks64();
        }

        if (animation_ticks == 0) continue;

        if (anim_enabled_ && animation != nullptr) {
            render_mutex_.lock();
            animation(animation_ticks);
            render_mutex_.unlock();

            if (!gui_enabled_ && camera_controller_) {
                SDL_WarpMouseInWindow(window_, width_/2, height_/2);
                controller_->prev_mouse_pos_.x = 0;
                controller_->prev_mouse_pos_.y = 0;
            }
        }

        ticks_at_last_anim = SDL_GetTicks64();
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
    cl::Program shadow_program = ocl_utils_.buildProgram(
        "gpu_shadow",
        "src/engine/GPU/kernel.cl"
    );
    cl::Program fragment_program = ocl_utils_.buildProgram(
        "gpu_fragment",
        "src/engine/GPU/kernel.cl"
    );

    std::cout << "Kernel Compilation Complete!" << std::endl;

    gpu_render_program gpu_render (cl::Kernel(render_program, "gpu_render"));
    gpu_lighting_program gpu_lighting (cl::Kernel(lighting_program, "gpu_lighting"));
    gpu_shadow_program gpu_shadow (cl::Kernel(shadow_program, "gpu_shadow"));
    gpu_fragment_program gpu_fragment (cl::Kernel(fragment_program, "gpu_fragment"));

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
                sizeof(float) * width_ * height_
        );

        lights_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(GPULight) * lights_.size()
        );

        normal_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(float3) * width_ * height_
        );

        position_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(float3) * width_ * height_
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

        color_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(uint32_t) * width_ * height_
        );

        tri_store_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(TriangleStore) * width_ * height_
        );

        t_buffer_ = cl::Buffer(
                ocl_utils_.context_,
                CL_MEM_READ_WRITE,
                sizeof(struct Vector3) * width_ * height_
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
                sizeof(float) * width_ * height_,
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
            if ((light->getType() != LightType::POINT && light->getType() != LightType::DIRECTIONAL) || !light->model_enabled_) continue;

            render_mutex_.lock();

            Object model = light->getModel();
            if (model.texture_w_ == 1 && model.texture_h_ == 1) {
                model.texture_[0] = Utils::linearToUint(light->color_[0], light->color_[1], light->color_[2]);
            }

            model.position_ = light->position_;

            render_mutex_.unlock();

            renderObject(model, gpu_render);
        }

        gpu_fragment(
            cl::EnqueueArgs(ocl_utils_.queue_, cl::NDRange(width_ * height_)),
            tri_store_buffer_,
            t_buffer_,
            texture_buffer_,
            color_buffer_,
            normal_buffer_,
            position_buffer_,
            diffuse_buffer_,
            specular_buffer_,
            state_buffer_
        );

        cl::NDRange global {width_ * height_};

        bool first = true;
        cl::Buffer shadow_map;

        std::vector<float> shadow_map_cpu;
        shadow_map_cpu.assign(512 * 512, -INFINITY);

        for (auto& light : gpu_lights_) {
            cl::Buffer light_buffer = cl::Buffer(
                    ocl_utils_.context_,
                    CL_MEM_READ_WRITE,
                    sizeof(GPULight)
            );

            light.shadow_map_width_ = 512;
            light.shadow_map_height_ = 512;

            ocl_utils_.queue_.enqueueWriteBuffer(
                    light_buffer,
                    CL_TRUE,
                    0,
                    sizeof(GPULight),
                    &light
            );

            if (light.type_ == LightType::DIRECTIONAL) {
                shadow_map = cl::Buffer(
                        ocl_utils_.context_,
                        CL_MEM_READ_WRITE,
                        sizeof(float) * light.shadow_map_width_ * light.shadow_map_height_
                );

                ocl_utils_.queue_.enqueueWriteBuffer(
                        shadow_map,
                        CL_TRUE,
                        0,
                        sizeof(float) * light.shadow_map_width_ * light.shadow_map_height_,
                        shadow_map_cpu.data()
                );

                for (const auto &object: render_queue_) {
                    cl::NDRange dims {object->triangles_.size()};

                    cl::Buffer tris = cl::Buffer(
                            ocl_utils_.context_,
                            CL_MEM_READ_WRITE,
                            sizeof(Triangle) * object->triangles_.size()
                    );

                    ocl_utils_.queue_.enqueueWriteBuffer(
                            tris,
                            CL_TRUE,
                            0,
                            sizeof(Triangle) * object->triangles_.size(),
                            object->triangles_.data()
                    );

                    gpu_shadow(
                            cl::EnqueueArgs(ocl_utils_.queue_, dims),
                            tris,
                            shadow_map,
                            light_buffer,
                            state_buffer_,
                            zbuffer_buffer_
                    ).wait();
                }
            }

            gpu_lighting(
                    cl::EnqueueArgs(ocl_utils_.queue_, global),
                    pixels_buffer_,
                    shadow_map,
                    color_buffer_,
                    light_buffer,
                    state_buffer_,
                    diffuse_buffer_,
                    specular_buffer_,
                    normal_buffer_,
                    position_buffer_,
                    zbuffer_buffer_
            ).wait();
        }

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
        tri_store_buffer_,
        t_buffer_,
        lights_buffer_,
        state_buffer_,
        zbuffer_buffer_,
        texture_buffer_,
        normal_buffer_,
        position_buffer_,
        diffuse_buffer_,
        specular_buffer_,
        color_buffer_,
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