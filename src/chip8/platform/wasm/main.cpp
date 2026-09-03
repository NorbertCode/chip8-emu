#include "application.hpp"
#include "chip8.hpp"
#include "loader.hpp"
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

// The below code is to hijack dropping files and handle it as loading roms
extern "C" 
{
    void EMSCRIPTEN_KEEPALIVE pushDropFileEvent(const char* filename)
    {
        SDL_Event event;
        event.type = SDL_DROPFILE;
        event.drop.file = SDL_strdup(filename);

        SDL_PushEvent(&event);
    }
}

EM_JS(void, hijackDropZone, (), {
    window.addEventListener("dragenter", function(e) { e.preventDefault(); });
    window.addEventListener("dragover", function(e) { e.preventDefault(); });

    window.addEventListener("drop", function(e) {
        e.preventDefault();

        console.log("file dropped");

        if (e.dataTransfer.files.length === 0)
            return;

        let file = e.dataTransfer.files[0];

        console.log(file.name);

        let reader = new FileReader();
        reader.onload = function(event) {
            let data = new Uint8Array(event.target.result);
            FS.writeFile(file.name, data);

            try {
                Module.ccall('pushDropFileEvent', 'null', ['string'], [file.name]);
            }
            catch (err) {
                console.error("ccall failed");
            }
        };
        reader.readAsArrayBuffer(file);
    });
});

int main()
{
    try
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
        {
            std::cerr << "Error initializing SDL: " << SDL_GetError() << '\n';
            return 1;
        }

        hijackDropZone();

        front::Loader* loader = new front::Loader; // NOLINT: Object must stay on the heap to work with WASM
        loader->loadConfig("configs/cosmac.toml");

        auto onStorageWriteCallback = [loader](std::span<const std::uint8_t, 16> data) {
            loader->writeStorage(data);
        };

        core::Chip8* chip8 = new core::Chip8(loader->getQuirks(), loader->getMemoryConfig(), loader->getDisplayConfig(), onStorageWriteCallback); // NOLINT: Object must stay on the heap to work with WASM
        chip8->getStorage().setData(loader->readStorage());

        front::Application* app = new front::Application(*chip8, loader->getApplicationConfig()); // NOLINT: Object must stay on the heap to work with WASM
        app->reset();

        app->getInput().addOnEventCallback([loader, chip8, app](const SDL_Event& event) {
            if (event.type == SDL_DROPFILE)
            {
                char* droppedFile = event.drop.file;
                
                loader->loadRom(droppedFile);
                chip8->loadRom(loader->getRom());
                app->reset();

                SDL_free(droppedFile);
            }
        });

        emscripten_set_main_loop_arg(mainLoop, app, 0, 1);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}