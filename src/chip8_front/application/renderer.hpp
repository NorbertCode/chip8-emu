#pragma once
#include <vector>
#include <SDL2/SDL.h>

class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();

    void render(const std::vector<std::vector<bool>>& display);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* displayTexture;

    int width, height;
};