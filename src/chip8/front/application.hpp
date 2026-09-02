#pragma once
#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_set>
#include "chip8.hpp"
#include "components/renderer.hpp"
#include "components/input.hpp"
#include "components/audio.hpp"
#include "debugger/debugger.hpp"

namespace chip8::front
{
    struct ApplicationConfig
    {
        double loopFrequency = 0;
        double timerFrequency = 0;
        double displayFrequency = 0;
        double audioFrequency = 0;
        int windowWidth = 0;
        int windowHeight = 0;
        std::uint32_t foregroundColor = 0;
        std::uint32_t backgroundColor = 0;
        std::array<std::string, 16> keyMap{};
    };

    class Application
    {
    public:
        Application(core::Chip8& chip8, ApplicationConfig config);

        void reset();
        void chipStep();
        void tick();

        bool shouldQuit() const;

        bool isRunning() const;
        void setRunning(bool value);

        const std::unordered_set<std::uint16_t>& getBreakpoints() const;
        void addBreakpoint(std::uint16_t line);
        void removeBreakpoint(std::uint16_t line);

    private:
        Renderer renderer;
        Input input;
        Audio audio;
        Debugger debugger;

        std::reference_wrapper<core::Chip8> chip8;

        double processorTime;
        double timerTime;
        double displayTime;

        double processorAccumulator = 0.0;
        double timerAccumulator = 0.0;
        double displayAccumulator = 0.0;
        std::chrono::time_point<std::chrono::high_resolution_clock> previousTime = std::chrono::high_resolution_clock::now();

        bool running = false;
        std::unordered_set<std::uint16_t> breakpoints;

        ApplicationConfig applicationConfig;
    };
}