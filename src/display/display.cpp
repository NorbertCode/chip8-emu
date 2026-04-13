#include "display.hpp"

Display::Display(const std::uint8_t width, const std::uint8_t height)
{
    display.resize(width, std::vector<bool>(height));
}

const bool Display::getPixel(const std::uint8_t x, const std::uint8_t y) const
{
    return display[x][y];
}

const std::vector<std::vector<bool>>& Display::getDisplay() const
{
    return display;
}

const size_t Display::getWidth() const
{
    return display.size();
}

const size_t Display::getHeight() const
{
    return display[0].size();
}

bool Display::xorPixel(const std::uint8_t x, const std::uint8_t y, const bool value)
{
    bool collision = display[x][y] && !value;

    display[x][y] = display[x][y] != value;

    return collision;
}

bool Display::xorSprite(const std::uint8_t x, const std::uint8_t y, const std::vector<std::uint8_t> sprite)
{
    bool collision = false;

    for (size_t sprite_row_index = 0; sprite_row_index < sprite.size(); ++sprite_row_index)
    {
        std::uint8_t sprite_row = sprite[sprite_row_index];
        std::uint8_t display_row_index = y + sprite_row_index;

        for (size_t sprite_column_index = 0; sprite_column_index < 8; ++sprite_column_index)
        {
            bool sprite_pixel = ((sprite_row >> (7 - sprite_column_index)) & 0x1) > 0;
            collision = xorPixel(x + sprite_column_index, display_row_index, sprite_pixel) || collision;
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