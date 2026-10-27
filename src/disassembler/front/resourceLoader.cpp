#include "resourceLoader.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace disassembler::front
{
    std::vector<std::uint16_t> ResourceLoader::loadRom(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open input file\n");

        std::streamsize size = static_cast<std::streamsize>(std::filesystem::file_size(path));

        std::vector<std::uint8_t> buffer(size);
        file.read(reinterpret_cast<char*>(buffer.data()), size);

        file.close();

        std::vector<std::uint16_t> rom(buffer.size() / 2);
        for (size_t i = 0; i < rom.size(); ++i)
            rom[i] = static_cast<std::uint16_t>(buffer[i * 2] << 8 | buffer[i * 2 + 1]);

        return rom;
    }

    std::string ResourceLoader::loadConfig(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open input file\n");

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}