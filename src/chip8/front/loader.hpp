#pragma once
#include "application.hpp"
#include <filesystem>
#include <string_view>
#include <toml++/toml.hpp>

namespace chip8::front
{
    class Loader
    {
    public:
        std::span<const std::uint8_t> getRom() const;
        const std::filesystem::path& getRomPath() const;

        const ApplicationConfig& getApplicationConfig() const;
        const core::MemoryConfig& getMemoryConfig() const;
        const core::DisplayConfig& getDisplayConfig() const;
        const core::Quirks& getQuirks() const;

        void writeStorage(std::span<const std::uint8_t, 16> data) const;
        std::array<std::uint8_t, 16> readStorage() const;

        void loadRom(const std::filesystem::path& newRomPath);
        void loadConfig(const std::filesystem::path& configPath);

    private:
        std::vector<std::uint8_t> rom;
        std::filesystem::path romPath;

        ApplicationConfig applicationConfig{};
        core::MemoryConfig memoryConfig{};
        core::DisplayConfig displayConfig{};
        core::Quirks quirks{};

        std::array<std::string, 16> parseKeyMap(const toml::table& keyMap) const;
        core::ResolutionMode parseResolutionMode(std::string_view mode) const;
        core::LoresSpriteHandling parseLoresSpriteHandling(std::string_view handling) const;
    };
}