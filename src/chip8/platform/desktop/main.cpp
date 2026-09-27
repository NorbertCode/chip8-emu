#include <argparse/argparse.hpp>
#include <exception>
#include <iostream>
#include "application.hpp"
#include "loading/configParser.hpp"
#include "loading/resourceLoader.hpp"

using namespace chip8;

struct Args
{
    std::string romPath;
    std::string configPath;
};

Args parseArgs(const std::string& name, int argc, char** argv)
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

int main(int argc, char* argv[])
{
    try
    {
        Args args = parseArgs("CHIP8-EMU", argc, argv);

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cerr << "Error initializing SDL: " << SDL_GetError() << '\n';
            return 1;
        }

        front::Application app(
            front::ConfigParser::parseConfig(front::ResourceLoader::loadConfig(args.configPath)),
            front::Rom { args.romPath, front::ResourceLoader::loadRom(args.romPath) }
        );

        app.reset();

        app.attachCallbackToInput([&app](const SDL_Event& event) {
            if (event.type == SDL_DROPFILE)
            {
                char* droppedFile = event.drop.file;
                
                app.setRom(front::Rom { droppedFile, front::ResourceLoader::loadRom(droppedFile) });
                app.reset();

                SDL_free(droppedFile);
            }
        });

        while (!app.shouldQuit())
            app.tick();

        SDL_Quit();

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}