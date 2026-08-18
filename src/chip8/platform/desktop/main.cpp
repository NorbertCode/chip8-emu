#include <exception>
#include <iostream>
#include "desktopLoader.hpp"
#include "application/application.hpp"

using namespace chip8;

int main(int argc, char* argv[])
{
    try
    {
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
        app.run();

        SDL_Quit();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}