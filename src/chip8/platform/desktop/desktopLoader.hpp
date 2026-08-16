#pragma once
#include "application/application.hpp"
#include <filesystem>
#include <string_view>
#include <toml++/toml.hpp>

class DesktopLoader
{
public:
    DesktopLoader(const std::string& name, int argc, char** argv);

    std::span<const std::uint8_t> getRom() const;
    const std::filesystem::path& getRomPath() const;

    const ApplicationConfig& getApplicationConfig() const;
    const MemoryConfig& getMemoryConfig() const;
    const DisplayConfig& getDisplayConfig() const;
    const Quirks& getQuirks() const;

    void writeStorage(std::span<const std::uint8_t, 16> data) const;
    std::array<std::uint8_t, 16> readStorage() const;

private:
    std::vector<std::uint8_t> rom;
    std::filesystem::path romPath;

    ApplicationConfig applicationConfig{};
    MemoryConfig memoryConfig{};
    DisplayConfig displayConfig{};
    Quirks quirks{};

    void loadRom(const std::filesystem::path& romPath);
    void loadConfig(const std::filesystem::path& configPath);

    std::array<std::string, 16> parseKeyMap(const toml::table& keyMap) const;
    ResolutionMode parseResolutionMode(std::string_view mode) const;
    LoresSpriteHandling parseLoresSpriteHandling(std::string_view handling) const;
};