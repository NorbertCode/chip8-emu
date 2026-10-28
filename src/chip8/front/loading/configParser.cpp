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
        bool keymapExists = keymapTable != nullptr;
        bool defaultModeExists = result["display"]["default_mode"].is_string();
        bool loresSpriteHandlingExists = result["quirks"]["lores_sprite_handling"].is_string();

        configs.applicationConfig = {
            .loopFrequency = result["application"]["loop_frequency"].value_or(defaultConfigs.applicationConfig.loopFrequency),
            .timerFrequency = result["application"]["timer_frequency"].value_or(defaultConfigs.applicationConfig.timerFrequency),
            .displayFrequency = result["application"]["display_frequency"].value_or(defaultConfigs.applicationConfig.displayFrequency),
            .audioFrequency = result["application"]["audio_frequency"].value_or(defaultConfigs.applicationConfig.audioFrequency),
            .windowWidth = result["application"]["window_width"].value_or(defaultConfigs.applicationConfig.windowWidth),
            .windowHeight = result["application"]["window_height"].value_or(defaultConfigs.applicationConfig.windowHeight),
            .foregroundColor = result["application"]["foreground_color"].value_or(defaultConfigs.applicationConfig.foregroundColor),
            .backgroundColor = result["application"]["background_color"].value_or(defaultConfigs.applicationConfig.backgroundColor),
            .romsPath = std::filesystem::path(result["application"]["roms_path"].value_or(defaultConfigs.applicationConfig.romsPath.string())),
            .configsPath = std::filesystem::path(result["application"]["configs_path"].value_or(defaultConfigs.applicationConfig.configsPath.string())),
            .keyMap = keymapExists ? parseKeyMap(*keymapTable) : defaultConfigs.applicationConfig.keyMap
        };

        configs.memoryConfig = {
            .reservedEnd = result["memory"]["reserved_end"].value_or(defaultConfigs.memoryConfig.reservedEnd),
            .programEnd = result["memory"]["program_end"].value_or(defaultConfigs.memoryConfig.programEnd),
            .reservedReadOnly = result["memory"]["reserved_read_only"].value_or(defaultConfigs.memoryConfig.reservedReadOnly)
        };

        configs.displayConfig = {
            .width = result["display"]["width"].value_or(defaultConfigs.displayConfig.width),
            .height = result["display"]["height"].value_or(defaultConfigs.displayConfig.height),
            .defaultMode = defaultModeExists ? parseResolutionMode(result["display"]["default_mode"].value<std::string>().value()) : defaultConfigs.displayConfig.defaultMode
        };

        configs.quirks = {
            .vfReset = result["quirks"]["vf_reset"].value_or(defaultConfigs.quirks.vfReset),
            .indexIncrement = result["quirks"]["index_increment"].value_or(defaultConfigs.quirks.indexIncrement),
            .displayClipping = result["quirks"]["display_clipping"].value_or(defaultConfigs.quirks.displayClipping),
            .vyShifting = result["quirks"]["vy_shifting"].value_or(defaultConfigs.quirks.vyShifting),
            .vxJumping = result["quirks"]["vx_jumping"].value_or(defaultConfigs.quirks.vxJumping),
            .clearOnDisplayModeChange = result["quirks"]["clear_on_mode_change"].value_or(defaultConfigs.quirks.clearOnDisplayModeChange),
            .vfCollisionCounter = result["quirks"]["vf_collision_counter"].value_or(defaultConfigs.quirks.vfCollisionCounter),
            .loresWholePixelScrolling = result["quirks"]["lores_whole_pixel_scrolling"].value_or(defaultConfigs.quirks.loresWholePixelScrolling),
            .waitForVBlank = result["quirks"]["wait_for_vblank"].value_or(defaultConfigs.quirks.waitForVBlank),
            .loresSpriteHandling = loresSpriteHandlingExists ? parseLoresSpriteHandling(result["quirks"]["lores_sprite_handling"].value<std::string>().value()) : defaultConfigs.quirks.loresSpriteHandling
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