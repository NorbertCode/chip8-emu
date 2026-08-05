#pragma once
#include <array>
#include <string>
#include "chip8.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "audio.hpp"

struct ApplicationConfig
{
    double loopFrequency;
    double timerFrequency;
    double displayFrequency;
    double audioFrequency;
    std::uint32_t foregroundColor;
    std::uint32_t backgroundColor;
    std::array<std::string, 16> keyMap;
};

class Application
{
public:
    Application(Chip8 chip8, ApplicationConfig config);

    void run();

private:
    Renderer renderer;
    Input input;
    Audio audio;

    Chip8 chip8;

    double processorTime;
    double timerTime;
    double displayTime;

    ApplicationConfig applicationConfig;
};