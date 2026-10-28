#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace disassembler::front
{
    class ResourceLoader
    {
    public:
        static std::vector<std::uint16_t> loadRom(const std::filesystem::path& path);
        static std::string loadConfig(const std::filesystem::path& path);
    };
}