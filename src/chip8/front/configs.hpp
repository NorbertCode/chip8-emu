#pragma once
#include "memory/memory.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"
#include <cstdint>
#include <array>
#include <filesystem>
#include <string>

namespace chip8::front
{
    struct ApplicationConfig
    {
        double loopFrequency = 0;
        double timerFrequency = 0;
        double displayFrequency = 0;
        double audioFrequency = 0;
        int windowWidth = 0;
        int windowHeight = 0;
        std::uint32_t foregroundColor = 0;
        std::uint32_t backgroundColor = 0;
        std::filesystem::path romsPath = "roms/";
        std::filesystem::path configsPath = "configs/emulator";
        std::array<std::string, 16> keyMap{};
    };

    struct Configs
    {
        ApplicationConfig applicationConfig{};
        core::MemoryConfig memoryConfig{};
        core::DisplayConfig displayConfig{};
        core::Quirks quirks{};
    };
}