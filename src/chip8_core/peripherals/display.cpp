#include "display.hpp"

Display::Display(const DisplayConfig& displayConfig) 
    : width(displayConfig.width), height(displayConfig.height), mode(displayConfig.defaultMode)
{
    display.resize(width * height, 0x0);
}

bool Display::getPixel(std::uint8_t x, std::uint8_t y) const
{
    return display[y * width + x];
}

const std::vector<std::uint8_t>& Display::getDisplay() const
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

    size_t index = y * width + x;

    bool collision = display[index] && value;
    display[index] = ((display[index] == 0xFF) != value) * 0xFF;

    return collision;
}

bool Display::xorSprite(std::uint8_t x, std::uint8_t y, const std::vector<std::uint8_t>& sprite, bool clipping)
{
    bool collision = false;

    for (size_t spriteRowIndex = 0; spriteRowIndex < sprite.size(); ++spriteRowIndex)
        collision |= xorRow(x, y + spriteRowIndex, sprite[spriteRowIndex], clipping);

    return collision;
}

bool Display::xorHiresSprite(std::uint8_t x, std::uint8_t y, const std::vector<std::uint8_t>& sprite, bool clipping)
{
    bool collision = false;

    for (size_t spriteRowIndex = 0; spriteRowIndex < sprite.size() / 2; ++spriteRowIndex)
    {
        std::uint16_t row = (sprite[2 * spriteRowIndex] << 8) | sprite[2 * spriteRowIndex + 1];

        collision |= xorRow(x, y + spriteRowIndex, row, clipping);
    }

    return collision;
}

void Display::clear()
{
    for (size_t i = 0; i < display.size(); ++i)
        display[i] = 0;
}

void Display::setResolutionMode(const ResolutionMode& mode)
{
    this->mode = mode;
}

const ResolutionMode& Display::getResolutionMode() const
{
    return mode;
}

