#pragma once
#include "disassembler.hpp"
#include <filesystem>
#include <span>
#include <cstdint>
#include <string>
#include <vector>

namespace disassembler::front
{
    class DesktopLoader
    {
    public:
        DesktopLoader(const std::string& name, int argc, char** argv);

        std::span<const std::uint16_t> getInput() const;
        core::DisassemblerConfig getDisassemblerConfig() const;
        bool shouldPrettify() const;
        
    private:
        std::vector<std::uint16_t> input;
        core::DisassemblerConfig disassemblerConfig;
        bool prettify = false;

        void loadInput(const std::filesystem::path& inputPath);
        void loadConfig(const std::filesystem::path& configPath);
    };
}