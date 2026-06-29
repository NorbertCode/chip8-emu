#include "desktopLoader.hpp"
#include <fstream>
#include <argparse/argparse.hpp>

DesktopLoader::DesktopLoader(const std::string& name, int argc, char* argv[])
{
    argparse::ArgumentParser parser(name);

    parser.add_argument("rom")
        .help("path to the ROM file to execute");

    parser.add_argument("-c", "--config")
        .help("path to the TOML configuration file to use")
        .default_value("./configs/default.toml");

    try
    {
        parser.parse_args(argc, argv);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Failed to parse arguments. Perhaps you forgot to specify ROM file?");
    }
    
    std::string romPath = parser.get<std::string>("rom");
    std::string configPath = parser.get<std::string>("--config");

    loadRom(romPath);
    loadConfig(configPath);
}

const std::vector<std::uint8_t>& DesktopLoader::getRom() const
{
    return rom;
}

const ApplicationConfig& DesktopLoader::getApplicationConfig() const
{
    return applicationConfig;
}

const MemoryConfig& DesktopLoader::getMemoryConfig() const
{
    return memoryConfig;
}

const DisplayConfig& DesktopLoader::getDisplayConfig() const
{
    return displayConfig;
}

const Quirks& DesktopLoader::getQuirks() const
{
    return quirks;
}

void DesktopLoader::loadRom(const std::string& romPath)
{
    std::ifstream file(romPath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
        throw std::runtime_error("Failed to open ROM file");

    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> fileBuffer(size);
    file.read(reinterpret_cast<char*>(fileBuffer.data()), size);

    file.close();

    rom = fileBuffer;
}

void DesktopLoader::loadConfig(const std::string& configPath)
{
    toml::parse_result config;

    try
    {
        config = toml::parse_file(configPath);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Failed to parse configuration file");
    }

    toml::table* keymapTable = config["keymap"].as_table();

    if (!keymapTable)
        throw std::runtime_error("Missing keymap in config.");

    applicationConfig = {
        .loopFrequency = config["application"]["loop_frequency"].value_or<double>(600.0),
        .timerFrequency = config["application"]["timer_frequency"].value_or<double>(60.0),
        .displayFrequency = config["application"]["display_frequency"].value_or<double>(60.0),
        .audioFrequency = config["application"]["audio_frequency"].value_or<double>(880.0),
        .keyMap = parseKeyMap(*config["keymap"].as_table())
    };

    memoryConfig = {
        .reservedEnd = config["memory"]["reserved_end"].value_or<std::uint16_t>(0x200),
        .programEnd = config["memory"]["program_end"].value_or<std::uint16_t>(0x1000),
        .reservedReadOnly = config["memory"]["reserved_read_only"].value_or(true)
    };

    displayConfig = {
        .width = config["display"]["width"].value_or<std::uint8_t>(64),
        .height = config["display"]["height"].value_or<std::uint8_t>(32)
    };

    quirks = {
        .vfReset = config["quirks"]["vf_reset"].value_or(false),
        .indexIncrement = config["quirks"]["index_increment"].value_or(false),
        .displayClipping = config["quirks"]["display_clipping"].value_or(false),
        .vyShifting = config["quirks"]["vy_shifting"].value_or(false),
        .vxJumping = config["quirks"]["vx_jumping"].value_or(false),
    };
}

std::array<std::string, 16> DesktopLoader::parseKeyMap(const toml::table& keyMap) const
{
    std::array<std::string, 16> keyArray;

    for (auto it = keyMap.begin(); it != keyMap.end(); ++it)
    {
        size_t key = 0;
        auto [ptr, ec] = std::from_chars(it->first.begin(), it->first.end(), key, 16);

        std::string value = it->second.value_or("");

        if (ec != std::errc() || key >= 16 || value.length() != 1)
            throw std::runtime_error("Invalid key map configuration");

        keyArray[key] = it->second.value_or("");
    }

    return keyArray;
}