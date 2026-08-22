#include "debugger.hpp"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

namespace chip8::front
{
    Debugger::Debugger(SDL_Window& window, SDL_Renderer& renderer)
        : renderer(renderer)
    {
        ImGui::CreateContext();
        ImGui_ImplSDL2_InitForSDLRenderer(&window, &renderer);
        ImGui_ImplSDLRenderer2_Init(&renderer);
    }

    Debugger::~Debugger()
    {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void Debugger::processEvent(const SDL_Event& event)
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
    }

    void Debugger::onFrameBegin()
    {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();
    }

    void Debugger::onFrameEnd()
    {
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), &renderer.get());
    }
}