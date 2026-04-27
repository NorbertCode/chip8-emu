#pragma once
#include <array>
#include <cstdint>
#include <functional>

class Keyboard
{
public:
    bool getKey(const std::uint8_t keyCode) const;

    void keyDown(const std::uint8_t keyCode);
    void keyUp(const std::uint8_t keyCode);

    void setOnKeyPressed(std::function<void(const std::uint8_t)> callback);
    void clearOnKeyDown();
    
private:
    std::array<bool, 16> keys{};

    std::function<void(const std::uint8_t)> onKeyDown;
};