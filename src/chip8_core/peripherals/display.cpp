#include "display.hpp"

Display::Display(std::uint8_t width, std::uint8_t height)
{
    display.resize(width, std::vector<bool>(height));
}

bool Display::getPixel(std::uint8_t x, std::uint8_t y) const
{
    return display[x][y];
}

const std::vector<std::vector<bool>>& Display::getDisplay() const
{
    return display;
}

size_t Display::getWidth() const
{
    return display.size();
}

size_t Display::getHeight() const
{
    return display[0].size();
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

    bool collision = display[x][y] && value;

    display[x][y] = display[x][y] != value;

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
        for (size_t j = 0; j < display[i].size(); ++j)
            display[i][j] = false;
    }
}