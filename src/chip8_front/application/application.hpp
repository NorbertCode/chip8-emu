#pragma once
#include <array>
#include <functional>
#include <string>
#include "chip8.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "audio.hpp"

struct ApplicationConfig
{
    double loopFrequency = 0;
    double timerFrequency = 0;
    double displayFrequency = 0;
    double audioFrequency = 0;
    std::uint32_t foregroundColor = 0;
    std::uint32_t backgroundColor = 0;
    std::array<std::string, 16> keyMap{};
};

class Application
{
public:
    Application(Chip8& chip8, ApplicationConfig config);

    void run();

private:
    Renderer renderer;
    Input input;
    Audio audio;

    std::reference_wrapper<Chip8> chip8;

    double processorTime;
    double timerTime;
    double displayTime;

    ApplicationConfig applicationConfig;
};