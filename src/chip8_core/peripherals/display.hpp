#pragma once
#include <cstdint>
#include <vector>

struct DisplayConfig
{
    // Coordinates are taken from registers which are 8-bit, hence std::uint8_t
    std::uint8_t width;
    std::uint8_t height;
};

class Display
{
public:
    Display(const DisplayConfig& displayConfig);

    bool getPixel(std::uint8_t x, std::uint8_t y) const;
    const std::vector<std::uint8_t>& getDisplay() const;
    std::uint8_t getWidth() const;
    std::uint8_t getHeight() const;

    bool xorPixel(std::uint8_t x, std::uint8_t y, bool value, bool clipping); // Returns collision (if pixel was erased)
    bool xorSprite(std::uint8_t x, std::uint8_t y, const std::vector<std::uint8_t>& sprite, bool clipping); // Returns collision (if pixel was erased)
    void clear();

private:
    std::vector<std::uint8_t> display;

    std::uint8_t width, height;
};
