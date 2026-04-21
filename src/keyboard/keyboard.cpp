#include "keyboard.hpp"

bool Keyboard::getKey(const std::uint8_t keyCode) const
{
    return keys[keyCode];
}

void Keyboard::keyDown(const std::uint8_t keyCode)
{
    keys[keyCode] = true;
}

void Keyboard::keyUp(const std::uint8_t keyCode)
{
    keys[keyCode] = false;
}