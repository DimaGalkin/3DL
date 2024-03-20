#include "gui.hpp"

ThreeDL::GUI::GUI(SDL_Window* window, SDL_Renderer* renderer) {
    if (window == nullptr || renderer == nullptr) {
        return;
    }
}

void ThreeDL::GUI::startFrame() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ThreeDL::GUI::renderFrame() {
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

ThreeDL::GUI::~GUI() {
}