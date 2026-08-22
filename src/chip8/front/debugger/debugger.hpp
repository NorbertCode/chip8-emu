#pragma once
#include <SDL2/SDL.h>
#include <functional>

namespace chip8::front
{
    class Debugger
    {
    public:
        Debugger(SDL_Window& window, SDL_Renderer& renderer);
        ~Debugger();

        Debugger(const Debugger&) = delete;
        Debugger& operator=(const Debugger&) = delete;
        Debugger(Debugger&&) = delete;
        Debugger& operator=(Debugger&&) = delete;

        void processEvent(const SDL_Event& event);
        void onFrameBegin();
        void onFrameEnd();

    private:
        std::reference_wrapper<SDL_Renderer> renderer;
    };
}