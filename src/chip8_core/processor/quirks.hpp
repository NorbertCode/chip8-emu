#pragma once

enum class LoresSpriteHandling
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
    LoresSpriteHandling loresSpriteHandling;
};