#include "application.hpp"
#include "loading/configParser.hpp"
#include "loading/resourceLoader.hpp"
#include <SDL.h>
#include <emscripten.h>
#include <exception>
#include <iostream>
#include <string_view>

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

constexpr std::string_view DEFAULT_CONFIG = "configs/cosmac.toml";

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

        front::Application* app = new front::Application( // NOLINT: Object must stay on the heap to work with WASM
            front::ConfigParser::parseConfig(front::ResourceLoader::loadConfig(DEFAULT_CONFIG)),
            front::Rom{}
        ); 

        app->reset();

        app->attachCallbackToInput([app](const SDL_Event& event) {
            if (event.type == SDL_DROPFILE)
            {
                char* droppedFile = event.drop.file;
                
                app->setRom(front::Rom { droppedFile, front::ResourceLoader::loadRom(droppedFile) });
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