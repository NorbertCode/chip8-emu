#include "application.hpp"
#include "chip8.hpp"
#include "wasmLoader.hpp"
#include <SDL.h>
#include <emscripten.h>
#include <exception>
#include <iostream>

using namespace chip8;

void mainLoop(void* arg)
{
    auto app = static_cast<front::Application*>(arg);

    app->tick();

    if (app->shouldQuit())
    {
        emscripten_cancel_main_loop();
        SDL_Quit();
    }
}

int main()
{
    try
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
        {
            std::cerr << "Error initializing SDL: " << SDL_GetError() << '\n';
            return 1;
        }

        front::WasmLoader* loader = new front::WasmLoader(); // NOLINT: Object must stay on the heap to work with WASM
        loader->loadRom("roms/ibm.ch8");

        auto onStorageWriteCallback = [loader](std::span<const std::uint8_t, 16> data) {
            loader->writeStorage(data);
        };

        core::Chip8* chip8 = new core::Chip8(loader->getQuirks(), loader->getMemoryConfig(), loader->getDisplayConfig(), onStorageWriteCallback); // NOLINT: Object must stay on the heap to work with WASM
        chip8->getStorage().setData(loader->readStorage());
        chip8->loadRom(loader->getRom());

        front::Application* app = new front::Application(*chip8, loader->getApplicationConfig()); // NOLINT: Object must stay on the heap to work with WASM
        app->reset();

        emscripten_set_main_loop_arg(mainLoop, app, 0, 1);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}