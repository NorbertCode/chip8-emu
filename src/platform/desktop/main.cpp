#include <iostream>
#include "desktopLoader.hpp"
#include "application/application.hpp"

int main(int argc, char* argv[])
{
    DesktopLoader loader("CHIP8", argc, argv);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    Chip8 chip8(loader.getQuirks(), loader.getMemoryConfig(), loader.getDisplayConfig());
    chip8.loadRom(loader.getRom());

    Application app(chip8, loader.getApplicationConfig());
    app.run();

    SDL_Quit();
}