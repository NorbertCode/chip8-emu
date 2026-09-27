#include "resourceLoader.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace chip8::front
{
    std::vector<std::uint8_t> ResourceLoader::loadRom(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open ROM file\n");

        std::streamsize size = static_cast<std::streamsize>(std::filesystem::file_size(path));

        std::vector<std::uint8_t> rom(size);
        file.read(reinterpret_cast<char*>(rom.data()), size);

        return rom;
    }

    std::string ResourceLoader::loadConfig(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("Failed to open config file\n");

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }

    std::vector<std::filesystem::path> ResourceLoader::findAllRoms(const std::filesystem::path& path)
    {
        return findAllFiles(path, ROM_EXTENSION, "ROM");
    }

    std::vector<std::filesystem::path> ResourceLoader::findAllConfigs(const std::filesystem::path& path)
    {
        return findAllFiles(path, CONFIG_EXTENSION, "config");
    }

    void ResourceLoader::writeStorage(const std::filesystem::path& romPath, std::span<const std::uint8_t, 16> data)
    {
        std::filesystem::path rplPath = romPath;
        rplPath.replace_extension(".rpl");

        std::ofstream file(rplPath, std::ios::binary);
        if (!file.is_open())
            return; // No throwing, because this is used during a ROMs runtime - an invalid ROM should not crash the emulator

        file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size() * sizeof(std::uint8_t)));
    }

    std::array<std::uint8_t, 16> ResourceLoader::readStorage(const std::filesystem::path& romPath)
    {
        std::array<std::uint8_t, 16> data{};

        std::filesystem::path rplPath = romPath;
        rplPath.replace_extension(".rpl");

        std::ifstream file(rplPath, std::ios::binary);
        if (!file.is_open())
            return data; // No throwing, because this is used during a ROMs runtime - an invalid ROM should not crash the emulator

        file.read(reinterpret_cast<char*>(data.data()), 16);

        return data;
    }

    std::vector<std::filesystem::path> ResourceLoader::findAllFiles(const std::filesystem::path& path, std::string_view extension, std::string_view directoryType)
    {
        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
            throw std::runtime_error(std::format("Path to {} directory does not exist or is not a directory\n", directoryType));
    
        std::vector<std::filesystem::path> files;
    
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file() && entry.path().extension() == extension)
                files.push_back(entry);
        }

        return files;
    }
}