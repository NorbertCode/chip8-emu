#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>
#include <span>
#include <vector>
#include "resourceLoader.hpp"

using namespace disassembler;
using namespace disassembler::front;

class DisasmResourceLoaderTest : public ::testing::Test
{
protected:
    std::filesystem::path testDir; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)

    void SetUp() override
    {
        testDir = std::filesystem::temp_directory_path() / "disasm_test_dir";
        std::filesystem::create_directories(testDir);
    }

    void TearDown() override
    {
        std::filesystem::remove_all(testDir);
    }

    void createFile(const std::filesystem::path& path, std::string_view content)
    {
        std::ofstream file(path);
        file << content;
    }

    void createBinaryFile(const std::filesystem::path& path, std::span<const std::uint8_t> content)
    {
        std::ofstream file(path, std::ios::binary);
        file.write(reinterpret_cast<const char*>(content.data()), static_cast<std::streamsize>(content.size()));
    }
};

TEST_F(DisasmResourceLoaderTest, LoadRom_ValidFile_LoadedCorrectly)
{
    std::filesystem::path romPath = testDir / "test_rom.ch8";
    std::vector<std::uint16_t> expectedData = { 0x00E0, 0xA22A };
    std::vector<std::uint8_t> inputData = { 0x00, 0xE0, 0xA2, 0x2A };
    createBinaryFile(romPath, inputData);

    std::vector<std::uint16_t> loadedData = ResourceLoader::loadRom(romPath);

    EXPECT_EQ(loadedData, expectedData);
}

TEST_F(DisasmResourceLoaderTest, LoadRom_InvalidFile_ThrowsException)
{
    std::filesystem::path romPath = testDir / "non_existent_rom.ch8";

    EXPECT_THROW(ResourceLoader::loadRom(romPath), std::runtime_error);
}

TEST_F(DisasmResourceLoaderTest, LoadRom_EmptyFile_LoadedCorrectly)
{
    std::filesystem::path romPath = testDir / "empty_rom.ch8";
    createBinaryFile(romPath, {});

    std::vector<std::uint16_t> loadedData = ResourceLoader::loadRom(romPath);

    EXPECT_TRUE(loadedData.empty());
}

TEST_F(DisasmResourceLoaderTest, LoadRom_InvalidPermissions_ThrowsException)
{
    std::filesystem::path romPath = testDir / "restricted_rom.ch8";
    createBinaryFile(romPath, {});
    std::filesystem::permissions(romPath, std::filesystem::perms::none);

    EXPECT_THROW(ResourceLoader::loadRom(romPath), std::runtime_error);

    std::filesystem::permissions(romPath, std::filesystem::perms::all, std::filesystem::perm_options::add);
}

TEST_F(DisasmResourceLoaderTest, LoadConfig_ValidFile_LoadedCorrectly)
{
    std::filesystem::path configPath = testDir / "test_config.toml";
    std::string expectedContent = "key = \"value\"";
    createFile(configPath, expectedContent);

    std::string loadedContent = ResourceLoader::loadConfig(configPath);

    EXPECT_EQ(loadedContent, expectedContent);
}

TEST_F(DisasmResourceLoaderTest, LoadConfig_InvalidFile_ThrowsException)
{
    std::filesystem::path configPath = testDir / "non_existent_config.toml";

    EXPECT_THROW(ResourceLoader::loadConfig(configPath), std::runtime_error);
}

TEST_F(DisasmResourceLoaderTest, LoadConfig_EmptyFile_LoadedCorrectly)
{
    std::filesystem::path configPath = testDir / "empty_config.toml";
    createFile(configPath, "");

    std::string loadedContent = ResourceLoader::loadConfig(configPath);

    EXPECT_TRUE(loadedContent.empty());
}

TEST_F(DisasmResourceLoaderTest, LoadConfig_InvalidPermissions_ThrowsException)
{
    std::filesystem::path configPath = testDir / "restricted_config.toml";
    createFile(configPath, "key = \"value\"");
    std::filesystem::permissions(configPath, std::filesystem::perms::none);

    EXPECT_THROW(ResourceLoader::loadConfig(configPath), std::runtime_error);

    std::filesystem::permissions(configPath, std::filesystem::perms::all, std::filesystem::perm_options::add);
}