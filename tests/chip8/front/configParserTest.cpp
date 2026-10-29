#include <gtest/gtest.h>
#include "loading/configParser.hpp"
#include "configs.hpp"
#include "memory/memory.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"

using namespace chip8;
using namespace chip8::front;

TEST(ConfigParserTest, ParseConfig_FullValidConfig_GetsParsedCorrectly)
{
    std::string config = R"(
        [application]
        loop_frequency = 600
        timer_frequency = 60
        display_frequency = 60
        audio_frequency = 440
        window_width = 1200
        window_height = 800
        foreground_color = 0xFFFFFF
        background_color = 0x010101
        roms_path = "roms"
        configs_path = "configs/emulator"

        [memory]
        reserved_end = 0x200
        program_end = 0x1000
        reserved_read_only = true

        [display]
        width = 64
        height = 32
        default_mode = "hires"

        [quirks]
        vf_reset = false
        index_increment = true
        display_clipping = false
        vy_shifting = true
        vx_jumping = false
        clear_on_mode_change = true
        vf_collision_counter = false
        lores_whole_pixel_scrolling = true
        wait_for_vblank = false
        lores_sprite_handling = "draw_wide"

        [keymap]
        0 = "X"
        1 = "1"
        2 = "2"
        3 = "3"
        4 = "Q"
        5 = "W"
        6 = "E"
        7 = "A"
        8 = "S"
        9 = "D"
        A = "Z"
        B = "C"
        C = "4"
        D = "R"
        E = "F"
        F = "V"
    )";
    ApplicationConfig expectedApplicationConfig = {
        .loopFrequency = 600.0,
        .timerFrequency = 60.0,
        .displayFrequency = 60.0,
        .audioFrequency = 440.0,
        .windowWidth = 1200,
        .windowHeight = 800,
        .foregroundColor = 0xFFFFFF,
        .backgroundColor = 0x010101,
        .romsPath = "roms",
        .configsPath = "configs/emulator",
        .keyMap = { "X", "1", "2", "3", "Q", "W", "E", "A", "S", "D", "Z", "C", "4", "R", "F", "V" }
    };
    core::MemoryConfig expectedMemoryConfig = {
        .reservedEnd = 0x200,
        .programEnd = 0x1000,
        .reservedReadOnly = true
    };
    core::DisplayConfig expectedDisplayConfig = {
        .width = 64,
        .height = 32,
        .defaultMode = chip8::core::ResolutionMode::Hires
    };
    core::Quirks expectedQuirks = {
        .vfReset = false,
        .indexIncrement = true,
        .displayClipping = false,
        .vyShifting = true,
        .vxJumping = false,
        .clearOnDisplayModeChange = true,
        .vfCollisionCounter = false,
        .loresWholePixelScrolling = true,
        .waitForVBlank = false,
        .loresSpriteHandling = chip8::core::LoresSpriteHandling::DrawWide
    };

    Configs configs = ConfigParser::parseConfig(config);

    EXPECT_EQ(configs.applicationConfig, expectedApplicationConfig);
    EXPECT_EQ(configs.memoryConfig, expectedMemoryConfig);
    EXPECT_EQ(configs.displayConfig, expectedDisplayConfig);
    EXPECT_EQ(configs.quirks, expectedQuirks);
}

TEST(ConfigParserTest, ParseConfig_MinimalValidConfig_GetsParsedCorrectly)
{
    std::string config = "";
    Configs expectedConfigs = ConfigParser::getDefaultConfigs();

    Configs configs = ConfigParser::parseConfig(config);

    EXPECT_EQ(configs.applicationConfig, expectedConfigs.applicationConfig);
    EXPECT_EQ(configs.memoryConfig, expectedConfigs.memoryConfig);
    EXPECT_EQ(configs.displayConfig, expectedConfigs.displayConfig);
    EXPECT_EQ(configs.quirks, expectedConfigs.quirks);
}

TEST(ConfigParserTest, ParseConfig_UnableToParseConfig_ThrowsException)
{
    std::string config = R"(
        [application]
        loop_frequency = invalid_value
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}

TEST(ConfigParserTest, ParseConfig_KeyValueMoreThanOneChar_ThrowsException)
{
    std::string config = R"(
        [keymap]
        0 = "X"
        1 = "1"
        2 = "2"
        3 = "3"
        4 = "Q"
        5 = "W"
        6 = "E"
        7 = "A"
        8 = "S"
        9 = "D"
        A = "Z"
        B = "C"
        C = "4"
        D = "R"
        E = "F"
        F = "VV" # Invalid value (length > 1)
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}

TEST(ConfigParserTest, ParseConfig_KeyNotHex_ThrowsException)
{
    std::string config = R"(
        [keymap]
        0 = "X"
        1 = "1"
        2 = "2"
        3 = "3"
        4 = "Q"
        5 = "W"
        6 = "E"
        7 = "A"
        8 = "S"
        9 = "D"
        A = "Z"
        B = "C"
        C = "4"
        D = "R"
        E = "F"
        G = "V" # Invalid key not hex)
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}

TEST(ConfigParserTest, ParseConfig_KeyOutOfRange_ThrowsException)
{
    std::string config = R"(
        [keymap]
        0 = "X"
        1 = "1"
        2 = "2"
        3 = "3"
        4 = "Q"
        5 = "W"
        6 = "E"
        7 = "A"
        8 = "S"
        9 = "D"
        A = "Z"
        B = "C"
        C = "4"
        D = "R"
        E = "F"
        10 = "V" # Invalid key out of range
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}

TEST(ConfigParserTest, ParseConfig_PartialKeyMap_GetsParsedCorrectly)
{
    std::string config = R"(
        [keymap]
        0 = "X"
        1 = "1"
        2 = "2"
    )";

    Configs configs = ConfigParser::parseConfig(config);

    EXPECT_EQ(configs.applicationConfig.keyMap[0], "X");
    EXPECT_EQ(configs.applicationConfig.keyMap[1], "1");
    EXPECT_EQ(configs.applicationConfig.keyMap[2], "2");
    for (int i = 3; i < 16; ++i)
    {
        EXPECT_EQ(configs.applicationConfig.keyMap[i], ConfigParser::getDefaultConfigs().applicationConfig.keyMap[i]);
    }
}

TEST(ConfigParserTest, ParseConfig_InvalidResolutionMode_ThrowsException)
{
    std::string config = R"(
        [display]
        default_mode = "invalid_mode"
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}

TEST(ConfigParserTest, ParseConfig_InvalidLoresSpriteHandling_ThrowsException)
{
    std::string config = R"(
        [quirks]
        lores_sprite_handling = "invalid_handling"
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}

TEST(ConfigParserTest, ParseConfig_InvalidValueType_ThrowsException)
{
    std::string config = R"(
        [application]
        loop_frequency = "not_a_number"
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}