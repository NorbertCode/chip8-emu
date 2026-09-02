#pragma once
#include "debugger/debugWidget.hpp"
#include "debugger/debugger.hpp"
#include <SDL.h>
#include <functional>
#include <memory>
#include <vector>

namespace chip8::front
{
    class DebuggerBuilder
    {
    public:
        DebuggerBuilder(SDL_Window& window, SDL_Renderer& renderer);

        DebuggerBuilder& addWidget(std::unique_ptr<DebugWidget> widget);
        Debugger build();

    private:
        std::vector<std::unique_ptr<DebugWidget>> widgets;

        std::reference_wrapper<SDL_Window> window;
        std::reference_wrapper<SDL_Renderer> renderer;
    };
}