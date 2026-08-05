#include "application.hpp"
#include <chrono>

Application::Application(Chip8& chip8, ApplicationConfig config)
    : renderer(chip8.getDisplay().getWidth(), chip8.getDisplay().getHeight(), config.windowWidth, config.windowHeight, config.foregroundColor, config.backgroundColor), 
      input(chip8.getKeyboard(), config.keyMap),
      audio(config.audioFrequency),
      chip8(chip8), 
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

    while(chip8.get().getProcessor().isRunning())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsedTime = currentTime - previousTime;
        previousTime = currentTime;

        processorAccumulator += elapsedTime.count();
        timerAccumulator += elapsedTime.count();
        displayAccumulator += elapsedTime.count();

        input.handleEvents();

        if (input.shouldQuit()) 
            return;

        if (chip8.get().getProcessor().getSoundTimer() > 0)
            audio.enable();
        else
            audio.disable();

        while (processorAccumulator >= processorTime)
        {
            chip8.get().getProcessor().step();

            processorAccumulator -= processorTime;
        }

        while (timerAccumulator >= timerTime)
        {
            chip8.get().getProcessor().tickTimers();

            timerAccumulator -= timerTime;
        }

        while (displayAccumulator >= displayTime)
        {
            renderer.render(chip8.get().getDisplay().getDisplay());

            displayAccumulator -= displayTime;
        }
    }
}