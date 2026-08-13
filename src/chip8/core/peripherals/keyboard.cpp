#include "keyboard.hpp"

bool Keyboard::getKey(std::uint8_t keyCode) const
{
    if (keyCode >= keys.size())
        return false;

    return keys[keyCode];
}

void Keyboard::keyDown(std::uint8_t keyCode)
{
    if (keyCode < keys.size())
        keys[keyCode] = true;
}

void Keyboard::keyUp(std::uint8_t keyCode)
{
    if (keyCode < keys.size())
        keys[keyCode] = false;

    if (onKeyPressed)
        onKeyPressed(keyCode);
}

void Keyboard::setOnKeyPressed(std::function<void(std::uint8_t)> callback)
{
    onKeyPressed = std::move(callback);
}

void Keyboard::clearOnKeyPressed()
{
    onKeyPressed = nullptr;
}