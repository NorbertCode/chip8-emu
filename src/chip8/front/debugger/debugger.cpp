#include "debugger.hpp"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <utility>

namespace chip8::front
{
    Debugger::Debugger()
        : widgets(0),
        isValid(false) { }

    Debugger::Debugger(SDL_Window& window, SDL_Renderer& renderer, std::vector<std::unique_ptr<DebugWidget>> widgets)
        : widgets(std::move(widgets)),
        isValid(true)
    {
        ImGui::CreateContext();
        ImGui_ImplSDL2_InitForSDLRenderer(&window, &renderer);
        ImGui_ImplSDLRenderer2_Init(&renderer);
    }

    Debugger::~Debugger()
    {
        if (!isValid)
            return;

        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    Debugger::Debugger(Debugger&& other) noexcept
        : widgets(std::move(other.widgets)),
        isValid(std::exchange(other.isValid, false)) { }

    Debugger& Debugger::operator=(Debugger&& other) noexcept
    {
        if (this != &other)
        {
            widgets = std::move(other.widgets);
            isValid = other.isValid;

            other.isValid = false;
        }

        return *this;
    }

    void Debugger::processEvent(const SDL_Event& event)
    {
        if (isValid)
            ImGui_ImplSDL2_ProcessEvent(&event);
    }

    void Debugger::draw()
    {
        if (!isValid)
            return;

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        for (auto& widget : widgets)
            widget->render();
    }

    void Debugger::render(SDL_Renderer& renderer)
    {
        if (!isValid)
            return;

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), &renderer);
    }
}