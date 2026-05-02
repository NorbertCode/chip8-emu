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

    if (onKeyDown)
        onKeyDown(keyCode);
}

void Keyboard::keyUp(std::uint8_t keyCode)
{
    if (keyCode < keys.size())
        keys[keyCode] = false;
}

void Keyboard::setOnKeyPressed(std::function<void(std::uint8_t)> callback)
{
    onKeyDown = std::move(callback);
}

void Keyboard::clearOnKeyDown()
{
    onKeyDown = nullptr;
}