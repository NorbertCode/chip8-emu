#pragma once
#include <cstdint>
#include <vector>

class Display
{
public:
    // Coordinates are taken from registers which are 8-bit, hence std::uint8_t
    Display(std::uint8_t width, std::uint8_t height);

    bool getPixel(std::uint8_t x, std::uint8_t y) const;
    const std::vector<std::vector<bool>>& getDisplay() const;
    size_t getWidth() const;
    size_t getHeight() const;

    bool xorPixel(std::uint8_t x, std::uint8_t y, bool value, bool clipping); // Returns collision (if pixel was erased)
    bool xorSprite(std::uint8_t x, std::uint8_t y, const std::vector<std::uint8_t>& sprite, bool clipping); // Returns collision (if pixel was erased)
    void clear();

private:
    std::vector<std::vector<bool>> display;

};
