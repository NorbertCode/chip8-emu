#include "configParser.hpp"
#include "disassembler.hpp"
#include <format>
#include <toml++/toml.hpp>

namespace disassembler::front
{
    core::DisassemblerConfig ConfigParser::parseConfig(std::string_view config)
    {
        toml::table result;
    
        try
        {
            result = toml::parse(config);
        }
        catch(const toml::parse_error& e)
        {
            throw std::runtime_error(std::format("Failed to parse configuration file: {}\n", e.what()));
        }

        return core::DisassemblerConfig {
            .programStart = result["disassembler"]["program_start"].value_or<std::uint16_t>(0x200),
            .vyShifting = result["disassembler"]["vy_shifting"].value_or(false),
            .vxJumping = result["disassembler"]["vx_jumping"].value_or(false),
            .hiresOperations = result["disassembler"]["hires_operations"].value_or(false)
        };
    }
}