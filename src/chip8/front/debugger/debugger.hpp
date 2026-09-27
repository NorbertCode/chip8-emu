#pragma once
#include "debugger/debugWidget.hpp"
#include <SDL.h>
#include <memory>
#include <vector>
#include <unordered_set>

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

        const std::unordered_set<std::uint16_t>& getBreakpoints() const;
        bool hasBreakpoint(std::uint16_t line) const;
        void addBreakpoint(std::uint16_t line);
        void removeBreakpoint(std::uint16_t line);

        void processEvent(const SDL_Event& event);
        void draw();
        void render(SDL_Renderer& renderer);

    private:
        bool isValid = false; // Used to allow for moving and prevent double ImGui shutdown
        bool layoutInitialized = false;

        std::unordered_set<std::uint16_t> breakpoints;
        std::vector<std::unique_ptr<DebugWidget>> widgets;
    };
}