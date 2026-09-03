#include "renderer.hpp"
#include "peripherals/display.hpp"
#include <iostream>

namespace chip8::front
{
    Renderer::Renderer(int width, int height, int windowWidth, int windowHeight,  std::uint32_t foregroundColor, std::uint32_t backgroundColor) 
        : width(width), 
        height(height), 
        windowWidth(windowWidth),
        windowHeight(windowHeight),
        foregroundColor(foregroundColor), 
        backgroundColor(backgroundColor),
        window(SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN))
    {
        if (!window)
        {
            std::cerr << "Error creating window: " << SDL_GetError() << '\n';
            return;
        }
        SDL_SetWindowResizable(window, SDL_TRUE);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            std::cerr << "Error creating renderer: " << SDL_GetError() << '\n';
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

    int Renderer::getWidth() const
    {
        return width;
    }

    int Renderer::getHeight() const
    {
        return height;
    }

    void Renderer::setForegroundColor(std::uint32_t color)
    {
        foregroundColor = color;
    }

    void Renderer::setBackgroundColor(std::uint32_t color)
    {
        backgroundColor = color;
    }

    std::uint32_t Renderer::getForegroundColor() const
    {
        return foregroundColor;
    }

    std::uint32_t Renderer::getBackgroundColor() const
    {
        return backgroundColor;
    }

    SDL_Window& Renderer::getWindow() const
    {
        return *window;
    }

    SDL_Renderer& Renderer::getRenderer() const
    {
        return *renderer;
    }

    SDL_Texture& Renderer::getDisplayTexture() const
    {
        return *displayTexture;
    }

    void Renderer::clearRenderer()
    {
        SDL_RenderClear(renderer);
    }

    void Renderer::drawDisplay(const core::Display& display)
    {
        void* pixels = nullptr;
        int pitch = 0;

        if (display.getWidth() != width || display.getHeight() != height)
            rebuildTexture(display.getWidth(), display.getHeight());

        SDL_LockTexture(displayTexture, NULL, (void**)&pixels, &pitch);

        int pitchPixels = pitch / SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_ARGB8888);
        std::uint32_t* pixels32 = static_cast<std::uint32_t*>(pixels);

        for (int y = 0; y < height; ++y)
        {
            int bufferRow = y * pitchPixels;
            int displayRow = y * width;

            for (int x = 0; x < width; ++x)
                pixels32[bufferRow + x] = 0xFF000000 | (display.getDisplay()[displayRow + x] > 0 ? foregroundColor : backgroundColor);
        }

        SDL_UnlockTexture(displayTexture);
    }

    void Renderer::render()
    {
        SDL_RenderPresent(renderer);
    }

    void Renderer::rebuildTexture(int width, int height)
    {
        this->width = width;
        this->height = height;

        SDL_DestroyTexture(displayTexture);
        displayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    }
}