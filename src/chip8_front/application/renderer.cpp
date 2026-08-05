#include "renderer.hpp"
#include <iostream>

Renderer::Renderer(int width, int height, std::uint32_t foregroundColor, std::uint32_t backgroundColor) 
    : width(width), height(height), foregroundColor(foregroundColor), backgroundColor(backgroundColor)
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
}

void Renderer::render(const std::vector<std::uint8_t>& display)
{
    SDL_RenderClear(renderer);

    void* pixels = nullptr;
    int pitch = 0;

    SDL_LockTexture(displayTexture, NULL, (void**)&pixels, &pitch);

    int pitchPixels = pitch / SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_ARGB8888);
    std::uint32_t* pixels32 = static_cast<std::uint32_t*>(pixels);

    for (int y = 0; y < height; ++y)
    {
        int bufferRow = y * pitchPixels;
        int displayRow = y * width;

        for (int x = 0; x < width; ++x)
            pixels32[bufferRow + x] = 0xFF000000 | (display[displayRow + x] > 0 ? foregroundColor : backgroundColor);
    }

    SDL_UnlockTexture(displayTexture);

    SDL_RenderCopy(renderer, displayTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}