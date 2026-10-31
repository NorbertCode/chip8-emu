#include <gtest/gtest.h>
#include "configParser.hpp"

using namespace disassembler;
using namespace disassembler::front;

TEST(DisasmConfigParserTest, ParseConfig_FullValidConfig_GetsParsedCorrectly)
{
    std::string config = R"(
        [disassembler]
        program_start = 0x300
        vy_shifting = true
        vx_jumping = true
        hires_operations = true
    )";

    core::DisassemblerConfig expectedConfig {
        .programStart = 0x300,
        .vyShifting = true,
        .vxJumping = true,
        .hiresOperations = true
    };

    core::DisassemblerConfig parsedConfig = ConfigParser::parseConfig(config);

    EXPECT_EQ(parsedConfig.programStart, expectedConfig.programStart);
    EXPECT_EQ(parsedConfig.vyShifting, expectedConfig.vyShifting);
    EXPECT_EQ(parsedConfig.vxJumping, expectedConfig.vxJumping);
    EXPECT_EQ(parsedConfig.hiresOperations, expectedConfig.hiresOperations);
}

TEST(DisasmConfigParserTest, ParseConfig_MinimalValidConfig_GetsParsedCorrectly)
{
    std::string config = "";

    core::DisassemblerConfig parsedConfig = ConfigParser::parseConfig(config);

    EXPECT_EQ(parsedConfig.programStart, ConfigParser::getDefaultConfigs().programStart);
    EXPECT_EQ(parsedConfig.vyShifting, ConfigParser::getDefaultConfigs().vyShifting);
    EXPECT_EQ(parsedConfig.vxJumping, ConfigParser::getDefaultConfigs().vxJumping);
    EXPECT_EQ(parsedConfig.hiresOperations, ConfigParser::getDefaultConfigs().hiresOperations);
}

TEST(DisasmConfigParserTest, ParseConfig_InvalidConfig_ThrowsException)
{
    std::string config = R"(
        [disassembler]
        program_start = invalid_value
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}

TEST(DisasmConfigParserTest, ParseConfig_InvalidProgramStartValueType_ThrowsException)
{
    std::string config = R"(
        [disassembler]
        program_start = "invalid_value"
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}

TEST(DisasmConfigParserTest, ParseConfig_InvalidQuirkValueType_ThrowsException)
{
    std::string config = R"(
        [disassembler]
        vy_shifting = "not_a_boolean"
    )";

    EXPECT_THROW(ConfigParser::parseConfig(config), std::runtime_error);
}