#pragma once
#include <vector>
#include <cstdint>
#include <SDL2/SDL.h>

class Renderer
{
public:
    Renderer(int width, int height, int windowWidth, int windowHeight, std::uint32_t foregroundColor, std::uint32_t backgroundColor);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void render(const std::vector<std::uint8_t>& display);

private:
    int width, height;
    int windowWidth, windowHeight;
    std::uint32_t foregroundColor, backgroundColor;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* displayTexture;
};