#pragma once
#include "configs.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"
#include <string_view>
#include <toml++/toml.hpp>

namespace chip8::front
{
    class ConfigParser
    {
    public:
        static Configs parseConfig(std::string_view config);

    private:
        static std::array<std::string, 16> parseKeyMap(const toml::table& keyMap);
        static core::ResolutionMode parseResolutionMode(std::string_view mode);
        static core::LoresSpriteHandling parseLoresSpriteHandling(std::string_view handling);
    };
}