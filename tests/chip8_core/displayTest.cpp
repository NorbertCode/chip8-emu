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

TEST(DisplayTest, GetParameters_ReturnsCorrectValues)
{
    const std::uint8_t width = 64;
    const std::uint8_t height = 32;

    Display display(width, height);

    EXPECT_EQ(display.getWidth(), width);
    EXPECT_EQ(display.getHeight(), height);
    for (size_t i = 0; i < display.getDisplay().size(); ++i)
    {
        for (size_t j = 0; j < display.getDisplay()[0].size(); ++j)
            EXPECT_FALSE(display.getDisplay()[i][j]);
    }
}

TEST(DisplayTest, XorPixel_From0Value1InBounds_Is1NoCollision)
{
    Display display(4, 4);

    bool collision = display.xorPixel(0, 0, true, false);

    EXPECT_TRUE(display.getPixel(0, 0));
    EXPECT_FALSE(collision);
}

TEST(DisplayTest, XorPixel_From0Value0InBounds_Is0NoCollision)
{
    Display display(4, 4);

    bool collision = display.xorPixel(0, 0, false, false);

    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_FALSE(collision);
}

TEST(DisplayTest, XorPixel_From1Value0InBounds_Is1NoCollision) 
{
    Display display(4, 4);
    display.xorPixel(0, 0, true, false);

    bool collision = display.xorPixel(0, 0, false, false);

    EXPECT_TRUE(display.getPixel(0, 0));
    EXPECT_FALSE(collision);
}

TEST(DisplayTest, XorPixel_From1Value1InBounds_Is0Collision)
{
    Display display(4, 4);
    display.xorPixel(0, 0, true, false);

    bool collision = display.xorPixel(0, 0, true, false);

    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_TRUE(collision);
}

TEST(DisplayTest, XorPixel_OutOfBoundsHorizontalClippingFalse_WrapsAround)
{
    Display display(2, 2);

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
    Display display(2, 2);

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
    Display display(2, 2);

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
    Display display(2, 2);

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
    Display display(8, 8);
    const std::vector<std::vector<bool>> expected = {
        { 1, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    display.xorSprite(0, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_From0sInBoundsWithOffset_DrawsSprite)
{
    Display display(8, 6);
    const std::vector<std::vector<bool>> expected = {
        { 0, 0, 0, 0, 0, 0 }, // First column (index 0 is topmost)
        { 0, 0, 0, 0, 0, 0 }, // Second column
        { 1, 0, 0, 0, 0, 0 }, // Etc...
        { 0, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 }
    };

    display.xorSprite(2, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_CollisionInBounds_XorsExisting)
{
    Display display(4, 4);
    display.xorSprite(0, 0, sprite_square, false);
    const std::vector<std::vector<bool>> expected = {
        { 0, 1, 0, 0 },
        { 1, 0, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    };

    display.xorSprite(0, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_OutOfBounds_WrapsAround)
{
    Display display(4, 4);
    const std::vector<std::vector<bool>> expected = {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    };

    display.xorSprite(4, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_OnBorder_HalfWrapsAround)
{
    Display display(4, 4);
    const std::vector<std::vector<bool>> expected = {
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 },
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 }
    };

    display.xorSprite(2, 0, sprite_diagonal, false);

    EXPECT_EQ(display.getDisplay(), expected);
}

TEST(DisplayTest, XorSprite_OutOfBoundsClippingTrue_DoesNotDraw)
{
    Display display(4, 4);

    display.xorSprite(4, 0, sprite_diagonal, true);

    for (size_t i = 0; i < display.getWidth(); ++i)
        EXPECT_EQ(display.getDisplay()[i], std::vector<bool>({ 0, 0, 0, 0 }));
}

TEST(DisplayTest, Clear_SetsAllPixelTo0)
{
    Display display(4, 4);
    display.xorSprite(0, 0, sprite_diagonal, false);

    display.clear();

    for (size_t i = 0; i < display.getWidth(); ++i)
    {
        for (size_t j = 0; j < display.getHeight(); ++j)
            EXPECT_FALSE(display.getPixel(i, j));
    }
}