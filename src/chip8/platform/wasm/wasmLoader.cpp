#include "wasmLoader.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"
#include <fstream>

namespace chip8::front
{
    WasmLoader::WasmLoader()
    {
        applicationConfig = {
            .loopFrequency = 600.0,
            .timerFrequency = 60.0,
            .displayFrequency = 60.0,
            .audioFrequency = 440.0,
            .windowWidth = 1200,
            .windowHeight = 800,
            .foregroundColor = 0xFFFFFFFF,
            .backgroundColor = 0xFF000000,
            .keyMap = {
                "X", "1", "2", "3",
                "Q", "W", "E", "A",
                "S", "D", "Z", "C"
            }
        };

        memoryConfig = {
            .reservedEnd = 0x200,
            .programEnd = 0x1000,
            .reservedReadOnly = true
        };

        displayConfig = {
            .width = 64,
            .height = 32,
            .defaultMode = core::ResolutionMode::Hires
        };

        quirks = {
            .vfReset = true,
            .indexIncrement = true,
            .displayClipping = true,
            .vyShifting = true,
            .vxJumping = false,
            .clearOnDisplayModeChange = true,
            .vfCollisionCounter = false,
            .loresWholePixelScrolling = false,
            .waitForVBlank = true,
            .loresSpriteHandling = core::LoresSpriteHandling::NoOperation
        };
    }

    std::span<const std::uint8_t> WasmLoader::getRom() const
    {
        return rom;
    }

    const std::filesystem::path& WasmLoader::getRomPath() const
    {
        return romPath;
    }

    const ApplicationConfig& WasmLoader::getApplicationConfig() const
    {
        return applicationConfig;
    }

    const core::MemoryConfig& WasmLoader::getMemoryConfig() const
    {
        return memoryConfig;
    }

    const core::DisplayConfig& WasmLoader::getDisplayConfig() const
    {
        return displayConfig;
    }

    const core::Quirks& WasmLoader::getQuirks() const
    {
        return quirks;
    }

    void WasmLoader::writeStorage(std::span<const std::uint8_t, 16> data) const
    {
        std::filesystem::path rplPath = romPath;
        rplPath.replace_extension(".rpl");

        std::ofstream file(rplPath, std::ios::binary);
        if (!file.is_open())
            return;

        file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size() * sizeof(std::uint8_t)));

        file.close();
    }

    std::array<std::uint8_t, 16> WasmLoader::readStorage() const
    {
        std::array<std::uint8_t, 16> data{};

        std::filesystem::path rplPath = romPath;
        rplPath.replace_extension(".rpl");

        std::ifstream file(rplPath, std::ios::binary);
        if (!file.is_open())
            return data;

        file.read(reinterpret_cast<char*>(data.data()), 16);

        file.close();

        return data;
    }

    void WasmLoader::loadRom(const std::filesystem::path& newRomPath)
    {
        romPath = newRomPath;

        std::ifstream file(newRomPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
            throw std::runtime_error("Failed to open ROM file");

        auto size = file.tellg();
        file.seekg(0, std::ios::beg);

        rom.resize(size);
        file.read(reinterpret_cast<char*>(rom.data()), size);

        file.close();
    }
}