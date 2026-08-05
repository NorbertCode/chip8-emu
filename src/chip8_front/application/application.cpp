#include "application.hpp"
#include <chrono>

Application::Application(Chip8 chip8, ApplicationConfig config)
    : renderer(chip8.getDisplay().getWidth(), chip8.getDisplay().getHeight(), config.foregroundColor, config.backgroundColor), 
      input(chip8.getKeyboard(), config.keyMap),
      audio(config.audioFrequency),
      chip8(std::move(chip8)), 
      processorTime(1000.0 / config.loopFrequency), 
      timerTime(1000.0 / config.timerFrequency),
      displayTime(1000.0 / config.displayFrequency),
      applicationConfig(std::move(config)) { }

void Application::run()
{
    double processorAccumulator = 0.0;
    double timerAccumulator = 0.0;
    double displayAccumulator = 0.0;
    auto previousTime = std::chrono::high_resolution_clock::now();

    while(chip8.getProcessor().isRunning())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsedTime = currentTime - previousTime;
        previousTime = currentTime;

        processorAccumulator += elapsedTime.count();
        displayAccumulator += elapsedTime.count();

        input.handleEvents();

        if (input.shouldQuit()) 
            return;

        if (chip8.getProcessor().getSoundTimer() > 0)
            audio.enable();
        else
            audio.disable();

        while (processorAccumulator >= processorTime)
        {
            chip8.getProcessor().step();
            chip8.getProcessor().tickTimers();

            processorAccumulator -= processorTime;
        }

        while (timerAccumulator >= timerTime)
        {
            chip8.getProcessor().tickTimers();

            timerAccumulator -= timerTime;
        }

        while (displayAccumulator >= displayTime)
        {
            renderer.render(chip8.getDisplay().getDisplay());

            displayAccumulator -= displayTime;
        }
    }
}