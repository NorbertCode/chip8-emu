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
            .loopFrequency = getValue(result["application"], "loop_frequency", defaultConfigs.applicationConfig.loopFrequency),
            .timerFrequency = getValue(result["application"], "timer_frequency", defaultConfigs.applicationConfig.timerFrequency),
            .displayFrequency = getValue(result["application"], "display_frequency", defaultConfigs.applicationConfig.displayFrequency),
            .audioFrequency = getValue(result["application"], "audio_frequency", defaultConfigs.applicationConfig.audioFrequency),
            .windowWidth = getValue(result["application"], "window_width", defaultConfigs.applicationConfig.windowWidth),
            .windowHeight = getValue(result["application"], "window_height", defaultConfigs.applicationConfig.windowHeight),
            .foregroundColor = getValue(result["application"], "foreground_color", defaultConfigs.applicationConfig.foregroundColor),
            .backgroundColor = getValue(result["application"], "background_color", defaultConfigs.applicationConfig.backgroundColor),
            .romsPath = std::filesystem::path(getValue(result["application"], "roms_path", defaultConfigs.applicationConfig.romsPath.string())),
            .configsPath = std::filesystem::path(getValue(result["application"], "configs_path", defaultConfigs.applicationConfig.configsPath.string())),
            .keyMap = keymapExists ? parseKeyMap(*keymapTable) : defaultConfigs.applicationConfig.keyMap
        };

        configs.memoryConfig = {
            .reservedEnd = getValue(result["memory"], "reserved_end", defaultConfigs.memoryConfig.reservedEnd),
            .programEnd = getValue(result["memory"], "program_end", defaultConfigs.memoryConfig.programEnd),
            .reservedReadOnly = getValue(result["memory"], "reserved_read_only", defaultConfigs.memoryConfig.reservedReadOnly)
        };

        configs.displayConfig = {
            .width = getValue(result["display"], "width", defaultConfigs.displayConfig.width),
            .height = getValue(result["display"], "height", defaultConfigs.displayConfig.height),
            .defaultMode = defaultModeExists ? parseResolutionMode(getValue(result["display"], "default_mode", "")) : defaultConfigs.displayConfig.defaultMode // NOLINT(bugprone-unchecked-optional-access) Checked above
        };

        configs.quirks = {
            .vfReset = getValue(result["quirks"], "vf_reset", defaultConfigs.quirks.vfReset),
            .indexIncrement = getValue(result["quirks"], "index_increment", defaultConfigs.quirks.indexIncrement),
            .displayClipping = getValue(result["quirks"], "display_clipping", defaultConfigs.quirks.displayClipping),
            .vyShifting = getValue(result["quirks"], "vy_shifting", defaultConfigs.quirks.vyShifting),
            .vxJumping = getValue(result["quirks"], "vx_jumping", defaultConfigs.quirks.vxJumping),
            .clearOnDisplayModeChange = getValue(result["quirks"], "clear_on_mode_change", defaultConfigs.quirks.clearOnDisplayModeChange),
            .vfCollisionCounter = getValue(result["quirks"], "vf_collision_counter", defaultConfigs.quirks.vfCollisionCounter),
            .loresWholePixelScrolling = getValue(result["quirks"], "lores_whole_pixel_scrolling", defaultConfigs.quirks.loresWholePixelScrolling),
            .waitForVBlank = getValue(result["quirks"], "wait_for_vblank", defaultConfigs.quirks.waitForVBlank),
            .loresSpriteHandling = loresSpriteHandlingExists ? parseLoresSpriteHandling(getValue(result["quirks"], "lores_sprite_handling", "")) : defaultConfigs.quirks.loresSpriteHandling // NOLINT(bugprone-unchecked-optional-access) Checked above
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

            std::string errorMessage;

            if (ec != std::errc())
                errorMessage = std::format("Invalid key map configuration: key '{}' is not a valid hex digit", it->first.str());
            else if (key >= 16)
                errorMessage = std::format("Invalid key map configuration: key '{}' is out of range (must be 0-F)", it->first.str());
            else if (value.length() != 1)
                errorMessage = std::format("Invalid key map configuration: value for key '{}' must be a single character", it->first.str());

            if (!errorMessage.empty())
                throw std::runtime_error(errorMessage);

            keyArray[key] = it->second.value_or("");
        }

        return keyArray;
    }

    core::ResolutionMode ConfigParser::parseResolutionMode(std::string_view mode)
    {
        if (mode == "lores")
            return core::ResolutionMode::Lores;
        else if (mode == "hires")
            return core::ResolutionMode::Hires;
        else
            throw std::runtime_error("Invalid resolution mode configuration: must be 'lores' or 'hires'");
    }

    core::LoresSpriteHandling ConfigParser::parseLoresSpriteHandling(std::string_view handling)
    {
        if (handling == "draw_wide")
            return core::LoresSpriteHandling::DrawWide;
        else if (handling == "draw_tall")
            return core::LoresSpriteHandling::DrawTall;
        else if (handling == "no_operation")
            return core::LoresSpriteHandling::NoOperation;
        else
            throw std::runtime_error("Invalid lores sprite handling configuration: must be 'draw_wide', 'draw_tall', or 'no_operation'");
    }
}