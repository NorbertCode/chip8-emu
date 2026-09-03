#include "desktopLoader.hpp"
#include <filesystem>
#include <argparse/argparse.hpp>

namespace chip8::front
{
    DesktopLoader::DesktopLoader(const std::string& name, int argc, char** argv)
    {
        argparse::ArgumentParser parser(name);

        parser.add_argument("rom")
            .help("path to the ROM file to execute");

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
        
        loadRom(parser.get<std::string>("rom"));
        loadConfig(parser.get<std::string>("--config"));
    }
}