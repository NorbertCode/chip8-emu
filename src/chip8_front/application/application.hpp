#pragma once
#include "chip8.hpp"
#include "renderer.hpp"

struct ApplicationConfig
{
    double loopFrequency;
    double displayFrequency;
};

class Application
{
public:
    Application(Chip8& chip8, const ApplicationConfig& config);

    void run();

private:
    Chip8& chip8;

    Renderer renderer;

    const double processorTime;
    const double displayTime;
};