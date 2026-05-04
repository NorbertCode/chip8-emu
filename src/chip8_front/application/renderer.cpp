#include "renderer.hpp"
#include <iostream>
#include <cstdint>

Renderer::Renderer(int width, int height) : width(width), height(height)
{
    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        return;
    }

    displayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}

Renderer::~Renderer()
{
    SDL_DestroyTexture(displayTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::render(const std::vector<std::uint8_t>& display)
{
    SDL_RenderClear(renderer);

    void* pixels;
    int pitch;

    SDL_LockTexture(displayTexture, NULL, (void**)&pixels, &pitch);

    int pitchPixels = pitch / SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_ARGB8888);
    std::uint32_t* pixels32 = static_cast<std::uint32_t*>(pixels);

    for (size_t y = 0; y < height; ++y)
    {
        size_t bufferRow = y * pitchPixels;
        size_t displayRow = y * width;

        for (size_t x = 0; x < width; ++x)
            pixels32[bufferRow + x] = display[displayRow + x] > 0 ? 0xFFFFFFFF : 0xFF000000;
    }

    SDL_UnlockTexture(displayTexture);

    SDL_RenderCopy(renderer, displayTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}