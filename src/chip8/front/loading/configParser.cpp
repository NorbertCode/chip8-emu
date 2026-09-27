#include "configParser.hpp"
#include <charconv>
#include <format>
#include <string>
#include <string_view>

namespace chip8::front
{
    Configs ConfigParser::parseConfig(std::string_view config)
    {
        toml::table result;

        try
        {
            result = toml::parse(config);
        }
        catch (const toml::parse_error& e)
        {
            throw std::runtime_error(std::format("Failed to parse configuration file: {}\n", e.what()));
        }

        Configs configs;

        toml::table* keymapTable = result["keymap"].as_table();

        if (!keymapTable)
            throw std::runtime_error("Missing keymap in config.");

        configs.applicationConfig = {
            .loopFrequency = result["application"]["loop_frequency"].value_or<double>(600.0),
            .timerFrequency = result["application"]["timer_frequency"].value_or<double>(60.0),
            .displayFrequency = result["application"]["display_frequency"].value_or<double>(60.0),
            .audioFrequency = result["application"]["audio_frequency"].value_or<double>(880.0),
            .windowWidth = result["application"]["window_width"].value_or<int>(1200),
            .windowHeight = result["application"]["window_height"].value_or<int>(800),
            .foregroundColor = result["application"]["foreground_color"].value_or<std::uint32_t>(0xFFFFFF),
            .backgroundColor = result["application"]["background_color"].value_or<std::uint32_t>(0x0),
            .romsPath = result["application"]["roms_path"].value_or<std::string>("roms"),
            .configsPath = result["application"]["configs_path"].value_or<std::string>("configs/emulator"),
            .keyMap = parseKeyMap(*keymapTable)
        };

        configs.memoryConfig = {
            .reservedEnd = result["memory"]["reserved_end"].value_or<std::uint16_t>(0x200),
            .programEnd = result["memory"]["program_end"].value_or<std::uint16_t>(0x1000),
            .reservedReadOnly = result["memory"]["reserved_read_only"].value_or(true)
        };

        configs.displayConfig = {
            .width = result["display"]["width"].value_or<std::uint8_t>(64),
            .height = result["display"]["height"].value_or<std::uint8_t>(32),
            .defaultMode = parseResolutionMode(result["display"]["default_mode"].value_or("hires"))
        };

        configs.quirks = {
            .vfReset = result["quirks"]["vf_reset"].value_or(false),
            .indexIncrement = result["quirks"]["index_increment"].value_or(false),
            .displayClipping = result["quirks"]["display_clipping"].value_or(false),
            .vyShifting = result["quirks"]["vy_shifting"].value_or(false),
            .vxJumping = result["quirks"]["vx_jumping"].value_or(false),
            .clearOnDisplayModeChange = result["quirks"]["clear_on_mode_change"].value_or(true),
            .vfCollisionCounter = result["quirks"]["vf_collision_counter"].value_or(false),
            .loresWholePixelScrolling = result["quirks"]["lores_whole_pixel_scrolling"].value_or(false),
            .waitForVBlank = result["quirks"]["wait_for_vblank"].value_or(false),
            .loresSpriteHandling = parseLoresSpriteHandling(result["quirks"]["lores_sprite_handling"].value_or("draw_wide"))
        };

        return configs;
    }

    std::array<std::string, 16> ConfigParser::parseKeyMap(const toml::table& keyMap)
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

    core::ResolutionMode ConfigParser::parseResolutionMode(std::string_view mode)
    {
        if (mode == "lores")
            return core::ResolutionMode::Lores;
        else 
            return core::ResolutionMode::Hires;
    }

    core::LoresSpriteHandling ConfigParser::parseLoresSpriteHandling(std::string_view handling)
    {
        if (handling == "draw_wide")
            return core::LoresSpriteHandling::DrawWide;
        else if (handling == "draw_tall")
            return core::LoresSpriteHandling::DrawTall;
        else
            return core::LoresSpriteHandling::NoOperation;
    }
}