#pragma once
#include "peripherals/display.hpp"
#include <cstdint>
#include <SDL2/SDL.h>

namespace chip8::front
{
    class Renderer
    {
    public:
        Renderer(int width, int height, int windowWidth, int windowHeight, std::uint32_t foregroundColor, std::uint32_t backgroundColor);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        int getWidth() const;
        int getHeight() const;

        SDL_Window& getWindow() const;
        SDL_Renderer& getRenderer() const;
        SDL_Texture& getDisplayTexture() const;

        void clearRenderer();
        void drawDisplay(const core::Display& display); // Draws to displayTexture
        void render();
        void rebuildTexture(int width, int height);

    private:
        int width, height;
        int windowWidth, windowHeight;
        std::uint32_t foregroundColor, backgroundColor;

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* displayTexture;
    };
}