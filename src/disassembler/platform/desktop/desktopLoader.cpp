#include "desktopLoader.hpp"
#include "disassembler.hpp"
#include <argparse/argparse.hpp>
#include <cstdint>
#include <toml++/toml.hpp>
#include <exception>
#include <fstream>
#include <span>
#include <stdexcept>

namespace disassembler::front
{
    DesktopLoader::DesktopLoader(const std::string& name, int argc, char** argv)
    {
        argparse::ArgumentParser parser(name);

        parser.add_argument("input")
            .help("path to the ROM file to disassemble");

        parser.add_argument("-c", "--config")
            .help("path to the TOML configuration file to use")
            .default_value("./configs/disassembler/example.toml");

        parser.add_argument("-p", "--prettify")
            .help("should prettify the output - makes it unusable as input")
            .default_value(false)
            .implicit_value(true);

        try 
        {
            parser.parse_args(argc, argv);
        } 
        catch (const std::exception& e) 
        {
            throw std::runtime_error("Failed to parse arguments. Perhaps you forgot to specify input file?");
        }

        std::string inputPath = parser.get<std::string>("input");
        std::string configPath = parser.get<std::string>("--config");
        prettify = parser.get<bool>("--prettify");

        loadInput(inputPath);
        loadConfig(configPath);
    }

    std::span<const std::uint16_t> DesktopLoader::getInput() const
    {
        return input;
    }

    core::DisassemblerConfig DesktopLoader::getDisassemblerConfig() const
    {
        return disassemblerConfig;
    }

    bool DesktopLoader::shouldPrettify() const
    {
        return prettify;
    }

    void DesktopLoader::loadInput(const std::filesystem::path& inputPath)
    {
        std::ifstream file(inputPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
            throw std::runtime_error("Failed to open input file");

        auto size = file.tellg();
        file.seekg(0, std::ios::beg);
    
        std::vector<std::uint8_t> buffer(size);
        file.read(reinterpret_cast<char*>(buffer.data()), size);

        file.close();

        input.resize(buffer.size() / 2);
        for (size_t i = 0; i < input.size(); ++i)
            input[i] = static_cast<std::uint16_t>(buffer[i * 2] << 8 | buffer[i * 2 + 1]);
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

        disassemblerConfig = {
            .vyShifting = config["disassembler"]["vy_shifting"].value_or(false),
            .vxJumping = config["disassembler"]["vx_jumping"].value_or(false),
            .hiresOperations = config["disassembler"]["hires_operations"].value_or(false)
        };
    }
}