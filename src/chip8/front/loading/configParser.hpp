#pragma once
#include "configs.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"
#include <format>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <toml++/impl/node.hpp>
#include <toml++/impl/node_view.hpp>
#include <toml++/toml.hpp>

namespace chip8::front
{
    class ConfigParser
    {
    public:
        static Configs parseConfig(std::string_view config);
        static const Configs& getDefaultConfigs();

    private:
        template <typename T>
        static T getValue(toml::node_view<toml::node> table, std::string_view key, T defaultValue)
        {
            if (table[key])
            {
                std::optional<T> result = table[key].value<T>();

                if (result.has_value())
                    return result.value();

                throw std::runtime_error(std::format("Invalid {} configuration: value does not match expected type", key));
            }

            return defaultValue;
        }

        static std::array<std::string, 16> parseKeyMap(const toml::table& keyMap);
        static core::ResolutionMode parseResolutionMode(std::string_view mode);
        static core::LoresSpriteHandling parseLoresSpriteHandling(std::string_view handling);

        inline static const Configs defaultConfigs = {
            .applicationConfig = {
                .loopFrequency = 600.0,
                .timerFrequency = 60.0,
                .displayFrequency = 60.0,
                .audioFrequency = 440.0,
                .windowWidth = 1200,
                .windowHeight = 800,
                .foregroundColor = 0xFFFFFF,
                .backgroundColor = 0x0,
                .romsPath = "roms/",
                .configsPath = "configs/emulator",
                .keyMap = { "X", "1", "2", "3", "Q", "W", "E", "A", "S", "D", "Z", "C", "4", "R", "F", "V" }
            },
            .memoryConfig = {
                .reservedEnd = 0x200,
                .programEnd = 0x1000,
                .reservedReadOnly = true
            },
            .displayConfig = {
                .width = 64,
                .height = 32,
                .defaultMode = core::ResolutionMode::Hires
            },
            .quirks = {
                .vfReset = false,
                .indexIncrement = false,
                .displayClipping = false,
                .vyShifting = false,
                .vxJumping = false,
                .clearOnDisplayModeChange = true,
                .vfCollisionCounter = false,
                .loresWholePixelScrolling = false,
                .waitForVBlank = false,
                .loresSpriteHandling = core::LoresSpriteHandling::DrawWide
            }
        };
    };
}