#include "desktopLoader.hpp"
#include <filesystem>
#include <fstream>
#include <argparse/argparse.hpp>

namespace chip8::front
{
    DesktopLoader::DesktopLoader(const std::string& name, int argc, char** argv)
    {
        argparse::ArgumentParser parser(name);

        parser.add_argument("rom")
            .help("path to the ROM file to execute");

        parser.add_argument("-c", "--config")
            .help("path to the TOML configuration file to use")
            .default_value("./configs/emulator/example.toml");

        try
        {
            parser.parse_args(argc, argv);
        }
        catch(const std::exception& e)
        {
            std::stringstream error;

            error << "Failed to parse arguments: " << e.what() << "\n\n" << parser;

            throw std::runtime_error(error.str());
        }
        
        romPath = parser.get<std::string>("rom");
        std::string configPath = parser.get<std::string>("--config");

        loadRom(romPath);
        loadConfig(configPath);
    }

    std::span<const std::uint8_t> DesktopLoader::getRom() const
    {
        return rom;
    }

    const std::filesystem::path& DesktopLoader::getRomPath() const
    {
        return romPath;
    }

    const ApplicationConfig& DesktopLoader::getApplicationConfig() const
    {
        return applicationConfig;
    }

    const core::MemoryConfig& DesktopLoader::getMemoryConfig() const
    {
        return memoryConfig;
    }

    const core::DisplayConfig& DesktopLoader::getDisplayConfig() const
    {
        return displayConfig;
    }

    const core::Quirks& DesktopLoader::getQuirks() const
    {
        return quirks;
    }

    void DesktopLoader::writeStorage(std::span<const std::uint8_t, 16> data) const
    {
        std::filesystem::path rplPath = romPath;
        rplPath.replace_extension(".rpl");

        std::ofstream file(rplPath, std::ios::binary);
        if (!file.is_open())
            return;

        file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size() * sizeof(std::uint8_t)));

        file.close();
    }

    std::array<std::uint8_t, 16> DesktopLoader::readStorage() const
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

    void DesktopLoader::loadRom(const std::filesystem::path& romPath)
    {
        std::ifstream file(romPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
            throw std::runtime_error("Failed to open ROM file");

        auto size = file.tellg();
        file.seekg(0, std::ios::beg);

        rom.resize(size);
        file.read(reinterpret_cast<char*>(rom.data()), size);

        file.close();
    }

    void DesktopLoader::loadConfig(const std::filesystem::path& configPath)
    {
        toml::parse_result config;

        try
        {
            config = toml::parse_file(configPath.string());
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
            .windowWidth = config["application"]["window_width"].value_or<int>(800),
            .windowHeight = config["application"]["window_height"].value_or<int>(600),
            .foregroundColor = config["application"]["foreground_color"].value_or<std::uint32_t>(0xFFFFFF),
            .backgroundColor = config["application"]["background_color"].value_or<std::uint32_t>(0x0),
            .keyMap = parseKeyMap(*config["keymap"].as_table())
        };

        memoryConfig = {
            .reservedEnd = config["memory"]["reserved_end"].value_or<std::uint16_t>(0x200),
            .programEnd = config["memory"]["program_end"].value_or<std::uint16_t>(0x1000),
            .reservedReadOnly = config["memory"]["reserved_read_only"].value_or(true)
        };

        displayConfig = {
            .width = config["display"]["width"].value_or<std::uint8_t>(64),
            .height = config["display"]["height"].value_or<std::uint8_t>(32),
            .defaultMode = parseResolutionMode(config["display"]["default_mode"].value_or("hires"))
        };

        quirks = {
            .vfReset = config["quirks"]["vf_reset"].value_or(false),
            .indexIncrement = config["quirks"]["index_increment"].value_or(false),
            .displayClipping = config["quirks"]["display_clipping"].value_or(false),
            .vyShifting = config["quirks"]["vy_shifting"].value_or(false),
            .vxJumping = config["quirks"]["vx_jumping"].value_or(false),
            .clearOnDisplayModeChange = config["quirks"]["clear_on_mode_change"].value_or(true),
            .vfCollisionCounter = config["quirks"]["vf_collision_counter"].value_or(false),
            .loresWholePixelScrolling = config["quirks"]["lores_whole_pixel_scrolling"].value_or(false),
            .waitForVBlank = config["quirks"]["wait_for_vblank"].value_or(false),
            .loresSpriteHandling = parseLoresSpriteHandling(config["quirks"]["lores_sprite_handling"].value_or("draw_wide"))
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

    core::ResolutionMode DesktopLoader::parseResolutionMode(std::string_view mode) const
    {
        if (mode == "lores")
            return core::ResolutionMode::Lores;
        else 
            return core::ResolutionMode::Hires;
    }

    core::LoresSpriteHandling DesktopLoader::parseLoresSpriteHandling(std::string_view handling) const
    {
        if (handling == "draw_wide")
            return core::LoresSpriteHandling::DrawWide;
        else if (handling == "draw_tall")
            return core::LoresSpriteHandling::DrawTall;
        else
            return core::LoresSpriteHandling::NoOperation;
    }
}