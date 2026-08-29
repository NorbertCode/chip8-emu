#pragma once
#include "debugger/debugWidget.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace chip8::front
{
    class Debugger
    {
    public:
        Debugger();
        Debugger(SDL_Window& window, SDL_Renderer& renderer, std::vector<std::unique_ptr<DebugWidget>> widgets);

        ~Debugger();

        Debugger(const Debugger&) = delete;
        Debugger& operator=(const Debugger&) = delete;

        Debugger(Debugger&& other) noexcept;
        Debugger& operator=(Debugger&& other) noexcept;

        void processEvent(const SDL_Event& event);
        void draw();
        void render(SDL_Renderer& renderer);

    private:
        bool isValid; // Used to allow for moving and prevent double ImGui shutdown

        std::vector<std::unique_ptr<DebugWidget>> widgets;
    };
}