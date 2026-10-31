#include <cstdint>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <span>
#include <string_view>
#include "loading/resourceLoader.hpp"

using namespace chip8;
using namespace chip8::front;

class ResourceLoaderTest : public ::testing::Test
{
protected:
    std::filesystem::path testDir; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)

    void SetUp() override
    {
        testDir = std::filesystem::temp_directory_path() / "chip8_test_dir";
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

TEST_F(ResourceLoaderTest, LoadRom_ValidFile_LoadedCorrectly)
{
    std::filesystem::path romPath = testDir / "test_rom.ch8";
    std::vector<std::uint8_t> expectedData = {0x00, 0xE0, 0xA2, 0x2A};
    createBinaryFile(romPath, expectedData);

    std::vector<std::uint8_t> loadedData = ResourceLoader::loadRom(romPath);

    EXPECT_EQ(loadedData, expectedData);
}

TEST_F(ResourceLoaderTest, LoadRom_InvalidFile_ThrowsException)
{
    std::filesystem::path romPath = testDir / "non_existent_rom.ch8";

    EXPECT_THROW(ResourceLoader::loadRom(romPath), std::runtime_error);
}

TEST_F(ResourceLoaderTest, LoadRom_EmptyFile_LoadedCorrectly)
{
    std::filesystem::path romPath = testDir / "empty_rom.ch8";
    createBinaryFile(romPath, {});

    std::vector<std::uint8_t> loadedData = ResourceLoader::loadRom(romPath);

    EXPECT_TRUE(loadedData.empty());
}

TEST_F(ResourceLoaderTest, LoadRom_InvalidPermissions_ThrowsException)
{
    std::filesystem::path romPath = testDir / "restricted_rom.ch8";
    createBinaryFile(romPath, {});
    std::filesystem::permissions(romPath, std::filesystem::perms::none);

    EXPECT_THROW(ResourceLoader::loadRom(romPath), std::runtime_error);

    std::filesystem::permissions(romPath, std::filesystem::perms::all, std::filesystem::perm_options::add);
}

TEST_F(ResourceLoaderTest, LoadConfig_ValidFile_LoadedCorrectly)
{
    std::filesystem::path configPath = testDir / "test_config.toml";
    std::string expectedContent = "key = \"value\"";
    createFile(configPath, expectedContent);

    std::string loadedContent = ResourceLoader::loadConfig(configPath);

    EXPECT_EQ(loadedContent, expectedContent);
}

TEST_F(ResourceLoaderTest, LoadConfig_InvalidFile_ThrowsException)
{
    std::filesystem::path configPath = testDir / "non_existent_config.toml";

    EXPECT_THROW(ResourceLoader::loadConfig(configPath), std::runtime_error);
}

TEST_F(ResourceLoaderTest, LoadConfig_EmptyFile_LoadedCorrectly)
{
    std::filesystem::path configPath = testDir / "empty_config.toml";
    createFile(configPath, "");

    std::string loadedContent = ResourceLoader::loadConfig(configPath);

    EXPECT_TRUE(loadedContent.empty());
}

TEST_F(ResourceLoaderTest, LoadConfig_InvalidPermissions_ThrowsException)
{
    std::filesystem::path configPath = testDir / "restricted_config.toml";
    createFile(configPath, "key = \"value\"");
    std::filesystem::permissions(configPath, std::filesystem::perms::none);

    EXPECT_THROW(ResourceLoader::loadConfig(configPath), std::runtime_error);

    std::filesystem::permissions(configPath, std::filesystem::perms::all, std::filesystem::perm_options::add);
}

TEST_F(ResourceLoaderTest, FindAllRoms_ValidDirectory_ReturnsCorrectFiles)
{
    std::filesystem::path rom1 = testDir / "rom1.ch8";
    std::filesystem::path rom2 = testDir / "rom2.ch8";
    std::filesystem::path nonRom = testDir / "not_a_rom.txt";

    createBinaryFile(rom1, {});
    createBinaryFile(rom2, {});
    createFile(nonRom, "This is not a ROM file.");

    std::vector<std::filesystem::path> foundRoms = ResourceLoader::findAllRoms(testDir);

    EXPECT_EQ(foundRoms.size(), 2);
    EXPECT_NE(std::find(foundRoms.begin(), foundRoms.end(), rom1), foundRoms.end());
    EXPECT_NE(std::find(foundRoms.begin(), foundRoms.end(), rom2), foundRoms.end());
}

TEST_F(ResourceLoaderTest, FindAllRoms_EmptyDirectory_ReturnsEmpty)
{
    std::vector<std::filesystem::path> foundRoms = ResourceLoader::findAllRoms(testDir);

    EXPECT_TRUE(foundRoms.empty());
}

TEST_F(ResourceLoaderTest, FindAllRoms_InvalidDirectory_ThrowsException)
{
    std::filesystem::path invalidDir = testDir / "non_existent_dir";

    EXPECT_THROW(ResourceLoader::findAllRoms(invalidDir), std::runtime_error);
}

TEST_F(ResourceLoaderTest, FindAllRoms_InvalidPermissions_ThrowsException)
{
    std::filesystem::path restrictedDir = testDir / "restricted_dir";
    std::filesystem::create_directory(restrictedDir);
    std::filesystem::permissions(restrictedDir, std::filesystem::perms::none);

    EXPECT_THROW(ResourceLoader::findAllRoms(restrictedDir), std::runtime_error);

    std::filesystem::permissions(restrictedDir, std::filesystem::perms::all, std::filesystem::perm_options::add);
}

TEST_F(ResourceLoaderTest, FindAllConfigs_ValidDirectory_ReturnsCorrectFiles)
{
    std::filesystem::path config1 = testDir / "config1.toml";
    std::filesystem::path config2 = testDir / "config2.toml";
    std::filesystem::path nonConfig = testDir / "not_a_config.txt";

    createFile(config1, "key1 = \"value1\"");
    createFile(config2, "key2 = \"value2\"");
    createFile(nonConfig, "This is not a config file.");

    std::vector<std::filesystem::path> foundConfigs = ResourceLoader::findAllConfigs(testDir);

    EXPECT_EQ(foundConfigs.size(), 2);
    EXPECT_NE(std::find(foundConfigs.begin(), foundConfigs.end(), config1), foundConfigs.end());
    EXPECT_NE(std::find(foundConfigs.begin(), foundConfigs.end(), config2), foundConfigs.end());
}

TEST_F(ResourceLoaderTest, FindAllConfigs_EmptyDirectory_ReturnsEmpty)
{
    std::vector<std::filesystem::path> foundConfigs = ResourceLoader::findAllConfigs(testDir);

    EXPECT_TRUE(foundConfigs.empty());
}

TEST_F(ResourceLoaderTest, FindAllConfigs_InvalidDirectory_ThrowsException)
{
    std::filesystem::path invalidDir = testDir / "non_existent_dir";

    EXPECT_THROW(ResourceLoader::findAllConfigs(invalidDir), std::runtime_error);
}

TEST_F(ResourceLoaderTest, FindAllConfigs_InvalidPermissions_ThrowsException)
{
    std::filesystem::path restrictedDir = testDir / "restricted_dir";
    std::filesystem::create_directory(restrictedDir);
    std::filesystem::permissions(restrictedDir, std::filesystem::perms::none);

    EXPECT_THROW(ResourceLoader::findAllConfigs(restrictedDir), std::runtime_error);

    std::filesystem::permissions(restrictedDir, std::filesystem::perms::all, std::filesystem::perm_options::add);
}

TEST_F(ResourceLoaderTest, WriteAndReadStorage_ValidData_WritesAndReadsCorrectly)
{
    std::filesystem::path romPath = testDir / "test_rom.ch8";
    createBinaryFile(romPath, {});

    std::array<std::uint8_t, 16> expectedData = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
    };

    ResourceLoader::writeStorage(romPath, expectedData);
    std::array<std::uint8_t, 16> readData = ResourceLoader::readStorage(romPath);

    EXPECT_EQ(readData, expectedData);
}

TEST_F(ResourceLoaderTest, WriteStorage_NonExistentStorageFile_CreatesFile)
{
    std::filesystem::path romPath = testDir / "test_rom.ch8";
    std::array<std::uint8_t, 16> dataToWrite = {
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
        0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00
    };

    ResourceLoader::writeStorage(romPath, dataToWrite);

    std::filesystem::path rplPath = romPath;
    rplPath.replace_extension(".rpl");

    EXPECT_TRUE(std::filesystem::exists(rplPath));

    std::array<std::uint8_t, 16> readData = ResourceLoader::readStorage(romPath);
    EXPECT_EQ(readData, dataToWrite);
}

TEST_F(ResourceLoaderTest, WriteStorage_InvalidPermissions_DoesNotThrow)
{
    std::filesystem::path romPath = testDir / "test_rom.ch8";
    createBinaryFile(romPath, {});
    std::filesystem::permissions(testDir, std::filesystem::perms::none);

    std::array<std::uint8_t, 16> dataToWrite = {
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
        0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00
    };

    EXPECT_NO_THROW(ResourceLoader::writeStorage(romPath, dataToWrite));

    std::filesystem::permissions(testDir, std::filesystem::perms::all, std::filesystem::perm_options::add);
}

TEST_F(ResourceLoaderTest, ReadStorage_NonExistentStorageFile_ReturnsDefaultData)
{
    std::filesystem::path romPath = testDir / "test_rom.ch8";
    createBinaryFile(romPath, {});
    std::array<std::uint8_t, 16> expectedData{};

    std::array<std::uint8_t, 16> readData = ResourceLoader::readStorage(romPath);

    EXPECT_EQ(readData, expectedData);
}

TEST_F(ResourceLoaderTest, ReadStorage_InvalidPermissions_ReturnsDefaultData)
{
    std::filesystem::path romPath = testDir / "test_rom.ch8";
    createBinaryFile(romPath, {});
    std::filesystem::permissions(testDir, std::filesystem::perms::none);
    std::array<std::uint8_t, 16> expectedData{};

    std::array<std::uint8_t, 16> readData = ResourceLoader::readStorage(romPath);

    EXPECT_EQ(readData, expectedData);

    std::filesystem::permissions(testDir, std::filesystem::perms::all, std::filesystem::perm_options::add);
}