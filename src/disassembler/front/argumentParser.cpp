#include "argumentParser.hpp"
#include <argparse/argparse.hpp>

namespace disassembler::front
{
    Args ArgumentParser::parseArgs(const std::string& name, int argc, char** argv)
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
            std::stringstream error;

            error << "Failed to parse arguments: " << e.what() << "\n\n" << parser;

            throw std::runtime_error(error.str());
        }

        return Args {
            .romPath = parser.get<std::string>("input"),
            .configPath = parser.get<std::string>("--config"),
            .prettify = parser.get<bool>("--prettify")
        };
    }
}