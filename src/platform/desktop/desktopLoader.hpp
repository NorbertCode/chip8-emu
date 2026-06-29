#pragma once
#include "application/application.hpp"
#include <toml++/toml.hpp>

class DesktopLoader
{
public:
    DesktopLoader(const std::string& name, int argc, char* argv[]);

    const std::vector<std::uint8_t>& getRom() const;

    const ApplicationConfig& getApplicationConfig() const;
    const MemoryConfig& getMemoryConfig() const;
    const DisplayConfig& getDisplayConfig() const;
    const Quirks& getQuirks() const;

private:
    std::vector<std::uint8_t> rom;

    ApplicationConfig applicationConfig;
    MemoryConfig memoryConfig;
    DisplayConfig displayConfig;
    Quirks quirks;

    void loadRom(const std::string& romPath);
    void loadConfig(const std::string& configPath);

    std::array<std::string, 16> parseKeyMap(const toml::table& keyMap) const;
};