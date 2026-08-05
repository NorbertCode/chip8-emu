#pragma once
#include "application/application.hpp"
#include <toml++/toml.hpp>

class DesktopLoader
{
public:
    DesktopLoader(const std::string& name, int argc, char* argv[]);

    const std::vector<std::uint8_t>& getRom() const;
    const std::string& getRomPath() const;

    const ApplicationConfig& getApplicationConfig() const;
    const MemoryConfig& getMemoryConfig() const;
    const DisplayConfig& getDisplayConfig() const;
    const Quirks& getQuirks() const;

    void writeStorage(const std::array<std::uint8_t, 16>& data) const;
    std::array<std::uint8_t, 16> readStorage() const;

private:
    std::vector<std::uint8_t> rom;
    std::string romPath;

    ApplicationConfig applicationConfig{};
    MemoryConfig memoryConfig{};
    DisplayConfig displayConfig{};
    Quirks quirks{};

    void loadRom(const std::string& romPath);
    void loadConfig(const std::string& configPath);

    std::array<std::string, 16> parseKeyMap(const toml::table& keyMap) const;
    ResolutionMode parseResolutionMode(const std::string& mode) const;
    LoresSpriteHandling parseLoresSpriteHandling(const std::string& handling) const;
};