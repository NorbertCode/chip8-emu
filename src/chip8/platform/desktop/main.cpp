#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "application.hpp"
#include "loading/argumentParser.hpp"
#include "loading/configParser.hpp"
#include "loading/resourceLoader.hpp"

using namespace chip8;

int main(int argc, char* argv[])
{
    try
    {
        const std::vector<std::string> argsList(argv, argv + argc);
        const front::Args args = front::ArgumentParser::parseArgs("CHIP8-EMU", argsList);

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cerr << "Error initializing SDL: " << SDL_GetError() << '\n';
            return 1;
        }

        front::Application app(
            front::ConfigParser::parseConfig(front::ResourceLoader::loadConfig(args.configPath)),
            front::Rom { args.romPath, args.romPath.empty() ? std::vector<std::uint8_t>{} : front::ResourceLoader::loadRom(args.romPath) }
        );

        app.reset();

        app.attachCallbackToInput([&app](const SDL_Event& event) {
            if (event.type == SDL_DROPFILE)
            {
                char* droppedFile = event.drop.file;
                
                try
                {
                    app.setRom(front::Rom { droppedFile, front::ResourceLoader::loadRom(droppedFile) });
                    app.reset();
                }
                catch(const std::exception& e) 
                {
                    std::cerr << std::format("Invalid ROM file: {}\n", e.what()); // Ignore invalid files to not crash the emulator
                }

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