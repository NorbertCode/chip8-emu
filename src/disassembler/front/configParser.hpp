#pragma once
#include "disassembler.hpp"
#include <string_view>

namespace disassembler::front
{
    class ConfigParser
    {
    public:
        static core::DisassemblerConfig parseConfig(std::string_view config);
    };
}