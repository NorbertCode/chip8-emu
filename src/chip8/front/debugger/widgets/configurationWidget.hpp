#pragma once
#include "chip8.hpp"
#include "debugger/debugWidget.hpp"
#include <array>
#include <functional>

namespace chip8::front
{
    class ConfigurationWidget : public DebugWidget
    {
    public:
        ConfigurationWidget(core::Chip8& chip8);
        void render() override;

    private:
        std::reference_wrapper<core::Chip8> chip8;

        std::array<std::string, 3> loresSpriteHandlingNames = { "Draw Wide", "Draw Tall", "No Operation" };
        std::array<std::string, 2> resolutionModeNames = { "Lores", "Hires" };

        int resolutionWidth;
        int resolutionHeight;
    };
}