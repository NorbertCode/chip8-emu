#include <gtest/gtest.h>
#include "argumentParser.hpp"

using namespace disassembler;
using namespace disassembler::front;

TEST(DisasmArgumentParserTest, ParseArgs_ValidShortArgs_ParsedCorrectly)
{
    const std::vector<std::string> args = { "disassembler", "-c", "config.toml", "test.rom", "-p" };

    Args parsedArgs = ArgumentParser::parseArgs("disassembler", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "config.toml");
    EXPECT_TRUE(parsedArgs.prettify);
}

TEST(DisasmArgumentParserTest, ParseArgs_ValidLongArgs_ParsedCorrectly)
{
    const std::vector<std::string> args = { "disassembler", "--config", "config.toml", "test.rom", "--prettify" };

    Args parsedArgs = ArgumentParser::parseArgs("disassembler", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "config.toml");
    EXPECT_TRUE(parsedArgs.prettify);
}

TEST(DisasmArgumentParserTest, ParseArgs_DifferentOrderArgs_ParsedCorrectly)
{
    const std::vector<std::string> args = { "disassembler", "test.rom", "-p", "-c", "config.toml" };

    Args parsedArgs = ArgumentParser::parseArgs("disassembler", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "config.toml");
    EXPECT_TRUE(parsedArgs.prettify);
}

TEST(DisasmArgumentParserTest, ParseArgs_MissingOptionalArgs_DefaultsUsed)
{
    const std::vector<std::string> args = { "disassembler", "test.rom" };

    Args parsedArgs = ArgumentParser::parseArgs("disassembler", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "./configs/disassembler/example.toml");
    EXPECT_FALSE(parsedArgs.prettify);
}

TEST(DisasmArgumentParserTest, ParseArgs_InvalidArgs_ThrowsRuntimeError)
{
    const std::vector<std::string> args = { "disassembler", "--invalid" };

    EXPECT_THROW(ArgumentParser::parseArgs("disassembler", args), std::runtime_error);
}

TEST(DisasmArgumentParserTest, ParseArgs_MissingRomArg_ThrowsRuntimeError)
{
    const std::vector<std::string> args = { "disassembler", "-c", "config.toml" };

    EXPECT_THROW(ArgumentParser::parseArgs("disassembler", args), std::runtime_error);
}

TEST(DisasmArgumentParserTest, ParseArgs_MissingValueForConfigArg_UsesDefaultValue)
{
    const std::vector<std::string> args = { "disassembler", "test.rom", "-c" };

    Args parsedArgs = ArgumentParser::parseArgs("disassembler", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "./configs/disassembler/example.toml");
}

TEST(DisasmArgumentParserTest, ParseArgs_MissingValueForPrettifyArg_UsesDefaultValue)
{
    const std::vector<std::string> args = { "disassembler", "test.rom", "-p" };

    Args parsedArgs = ArgumentParser::parseArgs("disassembler", args);

    EXPECT_EQ(parsedArgs.romPath, "test.rom");
    EXPECT_EQ(parsedArgs.configPath, "./configs/disassembler/example.toml");
    EXPECT_TRUE(parsedArgs.prettify);
}

TEST(DisasmArgumentParserTest, ParseArgs_TooManyPositionalArgs_ThrowsRuntimeError)
{
    const std::vector<std::string> args = { "disassembler", "test.rom", "extra_arg" };

    EXPECT_THROW(ArgumentParser::parseArgs("disassembler", args), std::runtime_error);
}