#pragma once
#include "chip8.hpp"
#include "debugger/debugWidget.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"
#include <array>
#include <functional>
#include <string>
#include <utility>

namespace chip8::front
{
    class ConfigurationWidget : public DebugWidget
    {
    public:
        ConfigurationWidget(core::Chip8& chip8);
        void render() override;

    private:
        std::reference_wrapper<core::Chip8> chip8;

        std::array<std::pair<core::LoresSpriteHandling, std::string>, 3> loresSpriteHandlingNames {{ 
            { core::LoresSpriteHandling::DrawWide, "Draw Wide" }, 
            { core::LoresSpriteHandling::DrawTall, "Draw Tall" }, 
            { core::LoresSpriteHandling::NoOperation, "No Operation" }
        }};
        std::array<std::pair<core::ResolutionMode, std::string>, 2> resolutionModeNames {{ 
            { core::ResolutionMode::Lores, "Lores" }, 
            { core::ResolutionMode::Hires, "Hires" }
        }};

        int resolutionWidth;
        int resolutionHeight;
    };
}