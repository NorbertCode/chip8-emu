#pragma once
#include <functional>
#include <span>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <SDL.h>
#include <vector>
#include "peripherals/keyboard.hpp"

namespace chip8::front
{
    class Input
    {
    public:
        Input(core::Keyboard& keyboard, std::span<const std::string, 16> keys);

        void handleEvents();
        bool shouldQuit() const;

        void addOnEventCallback(std::function<void(const SDL_Event&)> callback);

    private:
        std::unordered_map<SDL_Scancode, std::uint8_t> keyMap;
        bool quit = false;

        std::reference_wrapper<core::Keyboard> keyboard;

        std::vector<std::function<void(const SDL_Event&)>> callbacks;
    };
}