#pragma once
#include <array>
#include <cstdint>

class Keyboard
{
public:
    bool getKey(const std::uint8_t keyCode) const;

    void keyDown(const std::uint8_t keyCode);
    void keyUp(const std::uint8_t keyCode);
    
private:
    std::array<bool, 16> keys;
};