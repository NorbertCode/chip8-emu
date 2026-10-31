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
            .programStart = getValue(result["disassembler"], "program_start", defaultConfigs.programStart),
            .vyShifting = getValue(result["disassembler"], "vy_shifting", defaultConfigs.vyShifting),
            .vxJumping = getValue(result["disassembler"], "vx_jumping", defaultConfigs.vxJumping),
            .hiresOperations = getValue(result["disassembler"], "hires_operations", defaultConfigs.hiresOperations)
        };
    }
}