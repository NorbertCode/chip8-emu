#pragma once
#include <array>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "peripherals/keyboard.hpp"

class Input
{
public:
    Input(Keyboard& keyboard, std::array<std::string, 16> keys);

    void handleEvents();
    bool shouldQuit() const;

private:
    std::unordered_map<SDL_Scancode, std::uint8_t> keyMap;
    bool quit = false;

    std::reference_wrapper<Keyboard> keyboard;
};