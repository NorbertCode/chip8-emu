#include "application.hpp"
#include "debugger/debuggerBuilder.hpp"
#include "debugger/widgets/breakpointsWidget.hpp"
#include "debugger/widgets/configurationWidget.hpp"
#include "debugger/widgets/flowControlWidget.hpp"
#include "debugger/widgets/memoryViewerWidget.hpp"
#include "debugger/widgets/disassemblyViewerWidget.hpp"
#include "debugger/widgets/registersViewWidget.hpp"
#include "debugger/widgets/spritePreviewWidget.hpp"
#include "debugger/widgets/stackViewerWidget.hpp"
#include "debugger/widgets/viewportWidget.hpp"
#include <chrono>
#include <cstdint>
#include <memory>
#include <span>

namespace chip8::front
{
    Application::Application(core::Chip8& chip8, ApplicationConfig config)
        : renderer(chip8.getDisplay().getWidth(), chip8.getDisplay().getHeight(), config.windowWidth, config.windowHeight, config.foregroundColor, config.backgroundColor), 
        input(chip8.getKeyboard(), std::span<const std::string, 16>(config.keyMap)),
        audio(config.audioFrequency),
        chip8(chip8), 
        processorTime(1000.0 / config.loopFrequency), 
        timerTime(1000.0 / config.timerFrequency),
        displayTime(1000.0 / config.displayFrequency),
        applicationConfig(std::move(config))
    {
        auto onEventCallback = [this](const SDL_Event& event) {
            debugger.processEvent(event);
        };

        input.setOnEventCallback(onEventCallback);

        debugger = DebuggerBuilder(renderer.getWindow(), renderer.getRenderer())
            .addWidget(std::make_unique<MemoryViewerWidget>(chip8.getMemory(), 4))
            .addWidget(std::make_unique<DisassemblyViewerWidget>(chip8.getMemory(), chip8.getProcessor()))
            .addWidget(std::make_unique<StackViewerWidget>(chip8.getProcessor()))
            .addWidget(std::make_unique<ViewportWidget>(renderer))
            .addWidget(std::make_unique<RegistersViewerWidget>(chip8.getProcessor()))
            .addWidget(std::make_unique<SpritePreviewWidget>(chip8.getProcessor(), chip8.getMemory()))
            .addWidget(std::make_unique<FlowControlWidget>(*this, true))
            .addWidget(std::make_unique<BreakpointsWidget>(*this))
            .addWidget(std::make_unique<ConfigurationWidget>(chip8))
            .build();
    }

    void Application::reset()
    {
        double processorAccumulator = 0.0;
        double timerAccumulator = 0.0;
        double displayAccumulator = 0.0;
        auto previousTime = std::chrono::high_resolution_clock::now();

        chip8.get().getDisplay().clear();
        chip8.get().getMemory().clear();
        chip8.get().getProcessor().reset();

        chip8.get().loadRom(chip8.get().getRom());
    }

    void Application::chipStep()
    {
        chip8.get().getProcessor().step();
    }

    void Application::tick()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsedTime = currentTime - previousTime;
        previousTime = currentTime;

        if (running)
            processorAccumulator += elapsedTime.count();

        timerAccumulator += elapsedTime.count();
        displayAccumulator += elapsedTime.count();

        input.handleEvents();

        if (shouldQuit()) 
            return;

        if (chip8.get().getProcessor().getSoundTimer() > 0)
            audio.enable();
        else
            audio.disable();

        while (running && processorAccumulator >= processorTime)
        {
            chipStep();

            processorAccumulator -= processorTime;

            if (breakpoints.contains(chip8.get().getProcessor().getProgramCounter()))
                running = false;
        }

        while (timerAccumulator >= timerTime)
        {
            chip8.get().getProcessor().tickTimers();

            timerAccumulator -= timerTime;
        }

        while (displayAccumulator >= displayTime)
        {
            renderer.clearRenderer();

            debugger.draw();
            renderer.drawDisplay(chip8.get().getDisplay().getDisplay());
            debugger.render(renderer.getRenderer());
            renderer.render();

            displayAccumulator -= displayTime;
        }
    }

    bool Application::shouldQuit() const
    {
        return input.shouldQuit();
    }

    bool Application::isRunning() const
    {
        return running;
    }

    void Application::setRunning(bool value)
    {
        running = value;
    }

    const std::unordered_set<std::uint16_t>& Application::getBreakpoints() const
    {
        return breakpoints;
    }

    void Application::addBreakpoint(std::uint16_t line)
    {
        breakpoints.insert(line);
    }

    void Application::removeBreakpoint(std::uint16_t line)
    {
        breakpoints.erase(line);
    }
}