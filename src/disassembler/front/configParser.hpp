#pragma once
#include "disassembler.hpp"
#include <string_view>
#include <format>
#include <toml++/toml.hpp>

namespace disassembler::front
{
    class ConfigParser
    {
    public:
        static core::DisassemblerConfig parseConfig(std::string_view config);

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

        inline static const core::DisassemblerConfig defaultConfigs = {
            .programStart = 0x200,
            .vyShifting = false,
            .vxJumping = false,
            .hiresOperations = false
        };
    };
}