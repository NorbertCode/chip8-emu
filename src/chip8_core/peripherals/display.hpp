#pragma once
#include <cstdint>
#include <vector>
#include <type_traits>

enum class ResolutionMode
{
    Lores,
    Hires
};

struct DisplayConfig
{
    // Coordinates are taken from registers which are 8-bit, hence std::uint8_t
    std::uint8_t width;
    std::uint8_t height;
    ResolutionMode defaultMode = ResolutionMode::Hires;
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
    bool xorHiresSprite(std::uint8_t x, std::uint8_t y, const std::vector<std::uint8_t>& sprite, bool clipping); // Same as above, but expects a 16x16 sprite
    void clear();

    void setResolutionMode(ResolutionMode mode);
    const ResolutionMode& getResolutionMode() const;

    void scrollLeft(std::uint8_t pixels);
    void scrollRight(std::uint8_t pixels);
    void scrollDown(std::uint8_t pixels);

private:
    std::vector<std::uint8_t> display;

    std::uint8_t width, height;
    ResolutionMode mode;

    template <typename T> requires std::is_integral_v<T>
    bool xorRow(std::uint8_t x, std::uint8_t y, const T& row, bool clipping)
    {
        bool collision = false;

        for (size_t columnIndex = 0; columnIndex < sizeof(T) * 8; ++columnIndex)
        {
            const bool pixel = ((row >> ((sizeof(T) * 8) - columnIndex - 1)) & 0x1) > 0;

            if (mode == ResolutionMode::Hires)
                collision |= xorPixel(x + columnIndex, y, pixel, clipping);
            else
            {
                const std::uint8_t doubleX = 2 * (x + columnIndex);
                const std::uint8_t doubleY = 2 * y;
            
                collision |= xorPixel(doubleX, doubleY, pixel, clipping);
                collision |= xorPixel(doubleX + 1, doubleY, pixel, clipping);
                collision |= xorPixel(doubleX, doubleY + 1, pixel, clipping);
                collision |= xorPixel(doubleX + 1, doubleY + 1, pixel, clipping);
            }
        }

        return collision;
    }
};
