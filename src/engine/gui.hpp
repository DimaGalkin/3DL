#pragma once

#include <SDL2/SDL.h>

#include <iostream>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

namespace ThreeDL {
    class GUI {
        public:
            GUI(SDL_Window* window, SDL_Renderer* renderer);

            void startFrame();
            void renderFrame();

            ~GUI();
        private:
    };
};