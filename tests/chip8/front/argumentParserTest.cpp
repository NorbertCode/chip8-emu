#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "loading/argumentParser.hpp"

using namespace chip8;
using namespace chip8::front;

TEST(ArgumentParserTest, ParseArgs_ValidShortArgs_ParsedCorrectly)
{
    const std::vector<std::string> args = { "chip8", "-r", "test.rom", "-c", "config.toml" };

    Args parsedArgs = ArgumentParser::parseArgs("chip8", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "config.toml");
}

TEST(ArgumentParserTest, ParseArgs_ValidLongArgs_ParsedCorrectly)
{
    const std::vector<std::string> args = { "chip8", "--rom", "test.rom", "--config", "config.toml" };

    Args parsedArgs = ArgumentParser::parseArgs("chip8", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "config.toml");
}

TEST(ArgumentParserTest, ParseArgs_DifferentOrderArgs_ParsedCorrectly)
{
    const std::vector<std::string> args = { "chip8", "-c", "config.toml", "-r", "test.rom" };

    Args parsedArgs = ArgumentParser::parseArgs("chip8", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "config.toml");
}

TEST(ArgumentParserTest, ParseArgs_MissingOptionalArgs_DefaultsUsed)
{
    const std::vector<std::string> args = { "chip8" };

    Args parsedArgs = ArgumentParser::parseArgs("chip8", args);

    EXPECT_EQ(parsedArgs.romPath, "");
    EXPECT_EQ(parsedArgs.configPath, "./configs/emulator/example.toml");
}

TEST(ArgumentParserTest, ParseArgs_InvalidArgs_ThrowsRuntimeError)
{
    const std::vector<std::string> args = { "chip8", "--invalid" };

    EXPECT_THROW(ArgumentParser::parseArgs("chip8", args), std::runtime_error);
}

TEST(ArgumentParserTest, ParseArgs_RomWithoutValue_UsesDefaultValue)
{
    const std::vector<std::string> args = { "chip8", "-r" };

    Args parsedArgs = ArgumentParser::parseArgs("chip8", args);

    EXPECT_EQ(parsedArgs.romPath, "");
}

TEST(ArgumentParserTest, ParseArgs_ConfigWithoutValue_UsesDefaultValue)
{
    const std::vector<std::string> args = { "chip8", "-c" };

    Args parsedArgs = ArgumentParser::parseArgs("chip8", args);

    EXPECT_EQ(parsedArgs.configPath, "./configs/emulator/example.toml");
}