#pragma once
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <unordered_set>
#include <vector>
#include "chip8.hpp"
#include "configs.hpp"
#include "components/renderer.hpp"
#include "components/input.hpp"
#include "components/audio.hpp"
#include "debugger/debugger.hpp"

namespace chip8::front
{

    struct Rom
    {
        std::filesystem::path path = "";
        std::vector<std::uint8_t> content{};
    };

    class Application
    {
    public:
        Application(Configs configs, Rom rom);

        void reset();
        void chipStep();
        void tick();

        bool shouldQuit() const;

        bool isRunning() const;
        void setRunning(bool value);

        void setRom(Rom newRom);

        const std::unordered_set<std::uint16_t>& getBreakpoints() const;
        void addBreakpoint(std::uint16_t line);
        void removeBreakpoint(std::uint16_t line);

        void attachCallbackToInput(std::function<void(const SDL_Event&)> callback);

    private:
        core::Chip8 chip8;

        Renderer renderer;
        Input input;
        Audio audio;
        Debugger debugger;

        Configs configs;
        Rom rom;

        double processorTime;
        double timerTime;
        double displayTime;

        double processorAccumulator = 0.0;
        double timerAccumulator = 0.0;
        double displayAccumulator = 0.0;
        std::chrono::time_point<std::chrono::high_resolution_clock> previousTime = std::chrono::high_resolution_clock::now();

        bool running = false;
        std::unordered_set<std::uint16_t> breakpoints;

        void writeStorage(std::span<const std::uint8_t, 16> data);
    };
}