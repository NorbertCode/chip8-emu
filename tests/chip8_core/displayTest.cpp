#include <gtest/gtest.h>
#include "peripherals/display.hpp"

const std::vector<std::uint8_t> sprite_diagonal = {
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000
};

const std::vector<std::uint8_t> sprite_square = {
    0b11000000,
    0b11000000
};

const std::vector<std::uint8_t> hires_sprite = {
    0b11000000, 0b00000000,
    0b00110000, 0b00000000,
    0b00001100, 0b00000000,
    0b00000011, 0b00000000,
    0b00000000, 0b11000000,
    0b00000000, 0b00110000,
    0b00000000, 0b00001100,
    0b00000000, 0b00000011,
    0b00000000, 0b00001100,
    0b00000000, 0b00110000,
    0b00000000, 0b11000000,
    0b00000011, 0b00000000,
    0b00001100, 0b00000000,
    0b00110000, 0b00000000,
    0b11000000, 0b00000000,
};

TEST(DisplayTest, GetParameters_ReturnsCorrectValues)
{
    const std::uint8_t width = 64;
    const std::uint8_t height = 32;

    Display display({width, height});

    EXPECT_EQ(display.getWidth(), width);
    EXPECT_EQ(display.getHeight(), height);
    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}

TEST(DisplayTest, XorPixel_From0Value1InBounds_Is1NoCollision)
{
    Display display({4, 4});

    bool collision = display.xorPixel(0, 0, true, false);

    EXPECT_TRUE(display.getPixel(0, 0));
    EXPECT_FALSE(collision);
}

TEST(DisplayTest, XorPixel_From0Value0InBounds_Is0NoCollision)
{
    Display display({4, 4});

    bool collision = display.xorPixel(0, 0, false, false);

    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_FALSE(collision);
}

TEST(DisplayTest, XorPixel_From1Value0InBounds_Is1NoCollision) 
{
    Display display({4, 4});
    display.xorPixel(0, 0, true, false);

    bool collision = display.xorPixel(0, 0, false, false);

    EXPECT_TRUE(display.getPixel(0, 0));
    EXPECT_FALSE(collision);
}

TEST(DisplayTest, XorPixel_From1Value1InBounds_Is0Collision)
{
    Display display({4, 4});
    display.xorPixel(0, 0, true, false);

    bool collision = display.xorPixel(0, 0, true, false);

    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_TRUE(collision);
}

TEST(DisplayTest, XorPixel_OutOfBoundsHorizontalClippingFalse_WrapsAround)
{
    Display display({2, 2});

    // . . # <- Would be here
    // . .
    // Moved to here:
    // # .
    // . .
    display.xorPixel(2, 0, true, false);

    EXPECT_TRUE(display.getPixel(0, 0));
    EXPECT_FALSE(display.getPixel(1, 0));
    EXPECT_FALSE(display.getPixel(0, 1));
    EXPECT_FALSE(display.getPixel(1, 1));
}

TEST(DisplayTest, XorPixel_OutOfBoundsVerticalClippingFalse_WrapsAround)
{
    Display display({2, 2});

    // . .
    // . .
    //   # <- Would be here
    // Move to here:
    // . #
    // . .
    display.xorPixel(1, 2, true, false);

    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_TRUE(display.getPixel(1, 0));
    EXPECT_FALSE(display.getPixel(0, 1));
    EXPECT_FALSE(display.getPixel(1, 1));
}

TEST(DisplayTest, XorPixel_OutOfBoundsHorizontalClippingTrue_DoesNotDraw)
{
    Display display({2, 2});

    // . . # <- Would be here
    // . .
    display.xorPixel(2, 0, true, true);

    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_FALSE(display.getPixel(1, 0));
    EXPECT_FALSE(display.getPixel(0, 1));
    EXPECT_FALSE(display.getPixel(1, 1));
}

TEST(DisplayTest, XorPixel_OutOfBoundsVerticalClippingTrue_DoesNotDraw)
{
    Display display({2, 2});

    // . .
    // . .
    //   # <- Would be here
    display.xorPixel(1, 2, true, true);

    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_FALSE(display.getPixel(1, 0));
    EXPECT_FALSE(display.getPixel(0, 1));
    EXPECT_FALSE(display.getPixel(1, 1));
}

TEST(DisplayTest, XorSprite_From0sInBounds_DrawsSprite)
{
    Display display({8, 8});
    const std::vector<std::uint8_t> expected = {
        0xFF, 0, 0, 0, 0, 0, 0, 0,
        0, 0xFF, 0, 0, 0, 0, 0, 0,
        0, 0, 0xFF, 0, 0, 0, 0, 0,
        0, 0, 0, 0xFF, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    display.xorSprite(0, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_From0sInBoundsWithOffset_DrawsSprite)
{
    Display display({8, 6});
    const std::vector<std::uint8_t> expected = {
        0, 0, 0xFF, 0, 0, 0, 0, 0,
        0, 0, 0, 0xFF, 0, 0, 0, 0,
        0, 0, 0, 0, 0xFF, 0, 0, 0,
        0, 0, 0, 0, 0, 0xFF, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    display.xorSprite(2, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_CollisionInBounds_XorsExisting)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);
    const std::vector<std::uint8_t> expected = {
        0, 0xFF, 0, 0,
        0xFF, 0, 0, 0,
        0, 0, 0xFF, 0,
        0, 0, 0, 0xFF
    };

    display.xorSprite(0, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_OutOfBounds_WrapsAround)
{
    Display display({4, 4});
    const std::vector<std::uint8_t> expected = {
        0xFF, 0, 0, 0,
        0, 0xFF, 0, 0,
        0, 0, 0xFF, 0,
        0, 0, 0, 0xFF
    };

    display.xorSprite(4, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_OnBorder_HalfWrapsAround)
{
    Display display({4, 4});
    const std::vector<std::uint8_t> expected = {
        0, 0, 0xFF, 0,
        0, 0, 0, 0xFF,
        0xFF, 0, 0, 0,
        0, 0xFF, 0, 0
    };

    display.xorSprite(2, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_OutOfBoundsClippingTrue_DoesNotDraw)
{
    Display display({4, 4});

    display.xorSprite(4, 0, sprite_diagonal, true);

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}

TEST(DisplayTest, XorSprite_Lores_DrawsTwiceAsBig)
{
    Display display({4, 4, ResolutionMode::Lores});
    const std::vector<std::uint8_t> expected = {
        0xFF, 0xFF, 0, 0,
        0xFF, 0xFF, 0, 0,
        0, 0, 0xFF, 0xFF,
        0, 0, 0xFF, 0xFF
    };

    display.xorSprite(0, 0, sprite_diagonal, true);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorHiresSprite_Hires_DrawsSprite)
{
    Display display({16, 16, ResolutionMode::Hires});
    const std::vector<std::uint8_t> expected = {
        0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    display.xorHiresSprite(0, 0, hires_sprite, true);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorHiresSprite_NoCollisionHires_ReturnsFalse)
{
    Display display({16, 16, ResolutionMode::Hires});

    bool collision = display.xorHiresSprite(0, 0, hires_sprite, true);

    EXPECT_FALSE(collision);
}

TEST(DisplayTest, XorHiresSprite_CollisionHires_ReturnsTrue)
{
    Display display({16, 16, ResolutionMode::Hires});
    display.xorHiresSprite(0, 0, hires_sprite, true);

    bool collision = display.xorHiresSprite(0, 0, hires_sprite, true);

    EXPECT_TRUE(collision);
}

TEST(DisplayTest, Clear_SetsAllPixelTo0)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_diagonal, false);

    display.clear();

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}

TEST(DisplayTest, ScrollLeft_InBounds_ScrollsDisplayLeft)
{
    Display display({4, 4});
    display.xorSprite(2, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0, 0xFF, 0xFF, 0,
        0, 0xFF, 0xFF, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    display.scrollLeft(1);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollLeft_AtEdge_ClipsSprite)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0xFF, 0, 0, 0,
        0xFF, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    display.scrollLeft(1);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollLeft_0Pixels_DoesNotScroll)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0xFF, 0xFF, 0, 0,
        0xFF, 0xFF, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    display.scrollLeft(0);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollLeft_DisplayWidth_ClearsScreen)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);

    display.scrollLeft(4);

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}

TEST(DisplayTest, ScrollLeft_AboveDisplayWidth_ClearsScreen)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);

    display.scrollLeft(5);

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}

TEST(DisplayTest, ScrollRight_InBounds_ScrollsDisplayRight)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0, 0, 0xFF, 0xFF,
        0, 0, 0xFF, 0xFF,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    display.scrollRight(2);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollRight_AtEdge_ClipsSprite)
{
    Display display({4, 4});
    display.xorSprite(2, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0, 0, 0, 0xFF,
        0, 0, 0, 0xFF,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    display.scrollRight(1);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollRight_0Pixels_DoesNotScroll)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0xFF, 0xFF, 0, 0,
        0xFF, 0xFF, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    display.scrollRight(0);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollRight_DisplayWidth_ClearsScreen)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);

    display.scrollRight(4);

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}

TEST(DisplayTest, ScrollRight_AboveDisplayWidth_ClearsScreen)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);

    display.scrollRight(5);

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}

TEST(DisplayTest, ScrollDown_InBounds_ScrollsDisplayDown)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0, 0, 0, 0,
        0xFF, 0xFF, 0, 0,
        0xFF, 0xFF, 0, 0,
        0, 0, 0, 0
    };

    display.scrollDown(1);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollDown_AtEdge_ClipsSprite)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0xFF, 0xFF, 0, 0,
    };

    display.scrollDown(3);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollDown_0Pixels_DoesNotScroll)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);
    std::vector<std::uint8_t> expected = {
        0xFF, 0xFF, 0, 0,
        0xFF, 0xFF, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    display.scrollDown(0);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, ScrollDown_DisplayHeight_ClearsScreen)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);

    display.scrollDown(4);

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}

TEST(DisplayTest, ScrollDown_AboveDisplayHeight_ClearsScreen)
{
    Display display({4, 4});
    display.xorSprite(0, 0, sprite_square, false);

    display.scrollDown(5);

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}