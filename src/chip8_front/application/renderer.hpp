#pragma once
#include <vector>
#include <cstdint>
#include <SDL2/SDL.h>

class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();

    void render(const std::vector<std::uint8_t>& display);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* displayTexture;

    int width, height;
};