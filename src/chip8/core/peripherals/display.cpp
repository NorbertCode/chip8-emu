#include "display.hpp"
#include <algorithm>

namespace chip8::core
{
    Display::Display(const DisplayConfig& displayConfig) 
        : width(displayConfig.width), height(displayConfig.height), mode(displayConfig.defaultMode)
    {
        display.resize(static_cast<size_t>(width * height), 0x0);
    }

    bool Display::getPixel(std::uint8_t x, std::uint8_t y) const
    {
        return display[y * width + x];
    }

    std::span<const std::uint8_t> Display::getDisplay() const
    {
        return display;
    }

    std::uint8_t Display::getWidth() const
    {
        return width;
    }

    std::uint8_t Display::getHeight() const
    {
        return height;
    }

    bool Display::xorPixel(std::uint8_t x, std::uint8_t y, bool value, bool clipping)
    {
        if (!clipping)
        {
            x %= getWidth();
            y %= getHeight();
        }
        else if (x >= getWidth() || y >= getHeight())
            return false;

        const size_t index = y * width + x;

        bool collision = display[index] && value;
        display[index] = ((display[index] == 0xFF) != value) * 0xFF;

        return collision;
    }

    int Display::xorSprite(std::uint8_t x, std::uint8_t y, std::span<const std::uint8_t> sprite, bool clipping)
    {
        int collisions = 0;

        for (size_t spriteRowIndex = 0; spriteRowIndex < sprite.size(); ++spriteRowIndex)
            collisions += xorRow(x, y + spriteRowIndex, sprite[spriteRowIndex], clipping);

        return collisions;
    }

    int Display::xorHiresSprite(std::uint8_t x, std::uint8_t y, std::span<const std::uint8_t> sprite, bool clipping)
    {
        int collisions = 0;

        for (size_t spriteRowIndex = 0; spriteRowIndex < sprite.size() / 2; ++spriteRowIndex)
        {
            const std::uint16_t row = (sprite[2 * spriteRowIndex] << 8) | sprite[2 * spriteRowIndex + 1];

            collisions += xorRow(x, y + spriteRowIndex, row, clipping);
        }

        return collisions;
    }

    void Display::clear()
    {
        for (size_t i = 0; i < display.size(); ++i)
            display[i] = 0;
    }

    void Display::setResolutionMode(ResolutionMode mode)
    {
        this->mode = mode;
    }

    const ResolutionMode& Display::getResolutionMode() const
    {
        return mode;
    }

    void Display::scrollLeft(std::uint8_t pixels)
    {
        std::ptrdiff_t shift = static_cast<std::ptrdiff_t>(pixels);

        for (size_t row = 0; row < height; ++row)
        {
            const std::ptrdiff_t rowStart = static_cast<std::ptrdiff_t>(row * width);
            const std::ptrdiff_t rowEnd = static_cast<std::ptrdiff_t>(rowStart + width);

            const auto remainingStart = std::shift_left(display.begin() + rowStart, display.begin() + rowEnd, shift);
            std::fill(remainingStart, display.begin() + rowEnd, 0);
        }
    }

    void Display::scrollRight(std::uint8_t pixels)
    {
        std::ptrdiff_t shift = static_cast<std::ptrdiff_t>(pixels);

        for (size_t row = 0; row < height; ++row)
        {
            const std::ptrdiff_t rowStart = static_cast<std::ptrdiff_t>(row * width);
            const std::ptrdiff_t rowEnd = static_cast<std::ptrdiff_t>(rowStart + width);

            const auto remainingEnd = std::shift_right(display.begin() + rowStart, display.begin() + rowEnd, shift);
            std::fill(display.begin() + rowStart, remainingEnd, 0);
        }
    }

    void Display::scrollDown(std::uint8_t pixels)
    {
        const auto remainingEnd = std::shift_right(display.begin(), display.end(), static_cast<std::ptrdiff_t>(pixels * width));
        std::fill(display.begin(), remainingEnd, 0);
    }
}