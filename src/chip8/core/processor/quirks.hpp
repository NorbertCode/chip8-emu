#pragma once
#include <cstdint>

namespace chip8::core
{
    enum class LoresSpriteHandling : std::uint8_t
    {
        DrawWide,   // When in lores mode draw hires sprites as 16x16
        DrawTall,   // When in lores mode draw hires sprites as 8x16
        NoOperation // When in lores mode do not draw hires sprites
    };

    struct Quirks
    {
        bool vfReset;
        bool indexIncrement;
        bool displayClipping;
        bool vyShifting;
        bool vxJumping;
        bool clearOnDisplayModeChange;
        bool vfCollisionCounter;
        bool loresWholePixelScrolling;
        bool waitForVBlank;
        LoresSpriteHandling loresSpriteHandling;
    };
}