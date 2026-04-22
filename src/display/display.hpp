#pragma once
#include <cstdint>
#include <vector>

class Display
{
public:
    // Coordinates are taken from registers which are 8-bit, hence std::uint8_t
    Display(const std::uint8_t width, const std::uint8_t height);

    bool getPixel(const std::uint8_t x, const std::uint8_t y) const;
    const std::vector<std::vector<bool>>& getDisplay() const;
    size_t getWidth() const;
    size_t getHeight() const;

    bool xorPixel(const std::uint8_t x, const std::uint8_t y, const bool value); // Returns collision (if pixel was erased)
    bool xorSprite(const std::uint8_t x, const std::uint8_t y, const std::vector<std::uint8_t> sprite); // Returns collision (if pixel was erased)
    void clear();

private:
    std::vector<std::vector<bool>> display;

};
