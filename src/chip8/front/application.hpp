#pragma once
#include <array>
#include <functional>
#include <string>
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

        void run();

    private:
        Renderer renderer;
        Input input;
        Audio audio;
        Debugger debugger;

        std::reference_wrapper<core::Chip8> chip8;

        double processorTime;
        double timerTime;
        double displayTime;

        ApplicationConfig applicationConfig;
    };
}