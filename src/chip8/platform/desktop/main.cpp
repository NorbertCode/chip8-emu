#define SDL_MAIN_HANDLED

#include <exception>
#include <iostream>
#include "desktopLoader.hpp"
#include "application.hpp"

using namespace chip8;

int main(int argc, char* argv[])
{
    try
    {
        SDL_SetMainReady(); // Prevents SDL from hijacking entry point on Windows (along with the define at the very top)

        front::DesktopLoader loader("CHIP8", argc, argv);

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cerr << "Error initializing SDL: " << SDL_GetError() << '\n';
            return 1;
        }

        auto onStorageWriteCallback = [&loader](std::span<const std::uint8_t, 16> data) {
            loader.writeStorage(data);
        };

        core::Chip8 chip8(loader.getQuirks(), loader.getMemoryConfig(), loader.getDisplayConfig(), onStorageWriteCallback);
        chip8.getStorage().setData(loader.readStorage());
        chip8.loadRom(loader.getRom());

        front::Application app(chip8, loader.getApplicationConfig());
        app.reset();

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