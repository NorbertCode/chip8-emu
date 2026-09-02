#include "debuggerBuilder.hpp"

namespace chip8::front
{
    DebuggerBuilder::DebuggerBuilder(SDL_Window& window, SDL_Renderer& renderer)
        : window(window), renderer(renderer) { }

    DebuggerBuilder& DebuggerBuilder::addWidget(std::unique_ptr<DebugWidget> widget)
    {
        widgets.push_back(std::move(widget));

        return *this;
    }

    Debugger DebuggerBuilder::build()
    {
        return Debugger(window, renderer, std::move(widgets));
    }
}