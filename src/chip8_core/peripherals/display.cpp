#include "display.hpp"

Display::Display(const DisplayConfig& displayConfig) : width(displayConfig.width), height(displayConfig.height)
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

    for (size_t sprite_row_index = 0; sprite_row_index < sprite.size(); ++sprite_row_index)
    {
        std::uint8_t sprite_row = sprite[sprite_row_index];
        std::uint8_t display_row_index = y + sprite_row_index;

        for (size_t sprite_column_index = 0; sprite_column_index < 8; ++sprite_column_index)
        {
            bool sprite_pixel = ((sprite_row >> (7 - sprite_column_index)) & 0x1) > 0;
            collision = xorPixel(x + sprite_column_index, display_row_index, sprite_pixel, clipping) || collision;
        }
    }

    return collision;
}

void Display::clear()
{
    for (size_t i = 0; i < display.size(); ++i)
    {
        display[i] = 0;
    }
}