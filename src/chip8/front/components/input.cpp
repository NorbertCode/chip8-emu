#include "input.hpp"

namespace chip8::front
{
    Input::Input(core::Keyboard& keyboard, std::span<const std::string, 16> keys) 
        : keyboard(keyboard)
    {
        for (size_t i = 0; i < keys.size(); ++i)
            keyMap[SDL_GetScancodeFromName(keys[i].data())] = i;
    }

    void Input::handleEvents()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0)
        {
            if (callback)
                callback(event);

            SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;

            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    return;
                
                case SDL_KEYDOWN:
                    scancode = event.key.keysym.scancode;

                    if (keyMap.contains(scancode))
                        keyboard.get().keyDown(keyMap.at(event.key.keysym.scancode));

                    break;

                case SDL_KEYUP:
                    scancode = event.key.keysym.scancode;

                    if (keyMap.contains(scancode))
                        keyboard.get().keyUp(keyMap.at(event.key.keysym.scancode));

                    break;
            }
        }
    }

    bool Input::shouldQuit() const
    {
        return quit;
    }

    void Input::setOnEventCallback(std::function<void(const SDL_Event&)> callback)
    {
        this->callback = std::move(callback);
    }
}