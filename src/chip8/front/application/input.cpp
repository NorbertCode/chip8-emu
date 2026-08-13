#include "input.hpp"

Input::Input(Keyboard& keyboard, std::array<std::string, 16> keys) 
    : keyboard(keyboard)
{
    for (size_t i = 0; i < keys.size(); ++i)
        keyMap[SDL_GetScancodeFromName(keys[i].c_str())] = i;
}

void Input::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
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