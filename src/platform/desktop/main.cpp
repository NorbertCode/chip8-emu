#include <iostream>
#include "desktopLoader.hpp"
#include "application/application.hpp"

int main(int argc, char* argv[])
{
    DesktopLoader loader("CHIP8", argc, argv);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << '\n';
        return 1;
    }

    auto onStorageWriteCallback = [&loader](const std::array<std::uint8_t, 16>& data) {
        loader.writeStorage(data);
    };

    Chip8 chip8(loader.getQuirks(), loader.getMemoryConfig(), loader.getDisplayConfig(), onStorageWriteCallback);
    chip8.getStorage().setData(loader.readStorage());
    chip8.loadRom(loader.getRom());

    Application app(chip8, loader.getApplicationConfig());
    app.run();

    SDL_Quit();
}