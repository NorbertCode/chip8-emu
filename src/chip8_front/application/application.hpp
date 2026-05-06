#pragma once
#include <array>
#include <string>
#include "chip8.hpp"
#include "renderer.hpp"
#include "input.hpp"

struct ApplicationConfig
{
    double loopFrequency;
    double displayFrequency;
    std::array<std::string, 16> keyMap;
};

class Application
{
public:
    Application(Chip8& chip8, const ApplicationConfig& config);

    void run();

private:
    Chip8& chip8;

    Renderer renderer;
    Input input;

    const double processorTime;
    const double displayTime;
};