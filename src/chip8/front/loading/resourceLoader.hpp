#pragma once
#include <array>
#include <cstdint>
#include <filesystem>
#include <span>
#include <string_view>
#include <vector>

namespace chip8::front
{
    class ResourceLoader
    {
    public:
        static std::vector<std::uint8_t> loadRom(const std::filesystem::path& path);
        static std::string loadConfig(const std::filesystem::path& path);

        static std::vector<std::filesystem::path> findAllRoms(const std::filesystem::path& path);
        static std::vector<std::filesystem::path> findAllConfigs(const std::filesystem::path& path);

        static void writeStorage(const std::filesystem::path& romPath, std::span<const std::uint8_t, 16> data);
        static std::array<std::uint8_t, 16> readStorage(const std::filesystem::path& romPath);

    private:
        static constexpr std::string_view ROM_EXTENSION = ".ch8";
        static constexpr std::string_view CONFIG_EXTENSION = ".toml";

        static std::vector<std::filesystem::path> findAllFiles(const std::filesystem::path& path, std::string_view extension, std::string_view directoryType);
    };
}