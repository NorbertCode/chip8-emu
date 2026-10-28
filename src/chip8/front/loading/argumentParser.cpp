#include "argumentParser.hpp"
#include <argparse/argparse.hpp>

namespace chip8::front
{
    Args ArgumentParser::parseArgs(const std::string& name, int argc, char** argv)
    {
        argparse::ArgumentParser parser(name);

        parser.add_argument("-r", "--rom")
            .help("path to the ROM file to execute")
            .default_value("");

        parser.add_argument("-c", "--config")
            .help("path to the TOML configuration file to use")
            .default_value("./configs/emulator/example.toml");

        try
        {
            parser.parse_args(argc, argv);
        }
        catch(const std::exception& e)
        {
            std::stringstream error;

            error << "Failed to parse arguments: " << e.what() << "\n\n" << parser;

            throw std::runtime_error(error.str());
        }
        
        return Args {
            .romPath = parser.get<std::string>("--rom"),
            .configPath = parser.get<std::string>("--config")
        };
    }
}