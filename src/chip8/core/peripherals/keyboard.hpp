#pragma once
#include <array>
#include <cstdint>
#include <functional>

namespace chip8::core
{
    class Keyboard
    {
    public:
        bool getKey(std::uint8_t keyCode) const;

        void keyDown(std::uint8_t keyCode);
        void keyUp(std::uint8_t keyCode);

        void setOnKeyPressed(std::function<void(std::uint8_t)> callback);
        void clearOnKeyPressed();
        
    private:
        std::array<bool, 16> keys{};

        std::function<void(std::uint8_t)> onKeyPressed;
    };
}