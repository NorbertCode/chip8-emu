#include "application.hpp"
#include "chip8.hpp"
#include "configs.hpp"
#include "components/input.hpp"
#include "components/renderer.hpp"
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
#include "loading/resourceLoader.hpp"
#include <chrono>
#include <cstdint>
#include <memory>
#include <span>
#include <utility>

namespace chip8::front
{
    Application::Application(Configs configs, Rom rom)
        : chip8(configs.quirks, configs.memoryConfig, configs.displayConfig, [this](std::span<const std::uint8_t, 16> data) { writeStorage(data); }),
        renderer(configs.displayConfig.width, configs.displayConfig.height, 
                 configs.applicationConfig.windowWidth, configs.applicationConfig.windowHeight, 
                 configs.applicationConfig.foregroundColor, configs.applicationConfig.backgroundColor),
        input(chip8.getKeyboard(), configs.applicationConfig.keyMap),
        audio(configs.applicationConfig.audioFrequency),
        processorTime(1000.0 / configs.applicationConfig.loopFrequency),
        timerTime(1000.0 / configs.applicationConfig.timerFrequency),
        displayTime(1000.0 / configs.applicationConfig.displayFrequency),
        configs(std::move(configs)),
        rom(std::move(rom))
    {
        chip8.getStorage().setData(ResourceLoader::readStorage(this->rom.path));
        chip8.loadRom(this->rom.content);

        auto onEventCallback = [this](const SDL_Event& event) {
            debugger.processEvent(event);
        };

        input.addOnEventCallback(onEventCallback);

        debugger = DebuggerBuilder(renderer.getWindow(), renderer.getRenderer())
            .addWidget(std::make_unique<MemoryViewerWidget>(chip8.getMemory(), 4))
            .addWidget(std::make_unique<DisassemblyViewerWidget>(chip8.getMemory(), chip8.getProcessor()))
            .addWidget(std::make_unique<StackViewerWidget>(chip8.getProcessor()))
            .addWidget(std::make_unique<ViewportWidget>(renderer))
            .addWidget(std::make_unique<RegistersViewerWidget>(chip8.getProcessor()))
            .addWidget(std::make_unique<SpritePreviewWidget>(chip8.getProcessor(), chip8.getMemory()))
            .addWidget(std::make_unique<FlowControlWidget>(*this, true))
            .addWidget(std::make_unique<BreakpointsWidget>(*this))
            .addWidget(std::make_unique<ConfigurationWidget>(chip8, renderer))
            .build();
    }

    void Application::reset()
    {
        processorAccumulator = 0.0;
        timerAccumulator = 0.0;
        displayAccumulator = 0.0;
        previousTime = std::chrono::high_resolution_clock::now();

        running = false;

        chip8.getDisplay().clear();
        chip8.getMemory().clear();
        chip8.getProcessor().reset();

        chip8.loadFont();
        chip8.loadRom(chip8.getRom());
    }

    void Application::chipStep()
    {
        chip8.getProcessor().step();
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

        if (chip8.getProcessor().getSoundTimer() > 0)
            audio.enable();
        else
            audio.disable();

        while (running && processorAccumulator >= processorTime)
        {
            chipStep();

            processorAccumulator -= processorTime;

            if (breakpoints.contains(chip8.getProcessor().getProgramCounter()))
                running = false;
        }

        while (timerAccumulator >= timerTime)
        {
            chip8.getProcessor().tickTimers();

            timerAccumulator -= timerTime;
        }

        while (displayAccumulator >= displayTime)
        {
            renderer.clearRenderer();

            debugger.draw();
            renderer.drawDisplay(chip8.getDisplay());
            debugger.render(renderer.getRenderer());
            renderer.render();

            chip8.getProcessor().resetWaitingForVBlank();

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

    void Application::setRom(Rom newRom)
    {
        rom = std::move(newRom);
        chip8.loadRom(rom.content);
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

    Input& Application::getInput()
    {
        return input;
    }

    void Application::writeStorage(std::span<const std::uint8_t, 16> data)
    {
        ResourceLoader::writeStorage(rom.path, data);
    }
}