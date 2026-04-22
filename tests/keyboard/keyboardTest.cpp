#include <gtest/gtest.h>
#include "keyboard/keyboard.hpp"

TEST(KeyboardTest, GetKey_Initially_ReturnsFalse)
{
    Keyboard keyboard;

    for (std::size_t i = 0; i < 16; ++i)
        EXPECT_FALSE(keyboard.getKey(i));
}

TEST(KeyboardTest, KeyDown_GetKey_ReturnsTrue)
{
    Keyboard keyboard;

    keyboard.keyDown(0xA);

    EXPECT_TRUE(keyboard.getKey(0xA));
}

TEST(KeyboardTest, KeyUp_GetKey_ReturnsFalse)
{
    Keyboard keyboard;

    keyboard.keyDown(0xA);
    keyboard.keyUp(0xA);

    EXPECT_FALSE(keyboard.getKey(0xA));
}

TEST(KeyboardTest, GetKey_InvalidKeyCode_ReturnsFalse)
{
    Keyboard keyboard;

    EXPECT_FALSE(keyboard.getKey(0x10));
    EXPECT_FALSE(keyboard.getKey(0xFF));
}

TEST(KeyboardTest, KeyDown_InvalidKeyCode_DoesNothing)
{
    Keyboard keyboard;

    keyboard.keyDown(0x10);
    keyboard.keyDown(0xFF);

    for (std::size_t i = 0; i < 16; ++i)
        EXPECT_FALSE(keyboard.getKey(i));
}

TEST(KeyboardTest, KeyUp_InvalidKeyCode_DoesNothing)
{
    Keyboard keyboard;

    keyboard.keyUp(0x10);
    keyboard.keyUp(0xFF);

    for (std::size_t i = 0; i < 16; ++i)
        EXPECT_FALSE(keyboard.getKey(i));
}
