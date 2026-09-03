#pragma once
#include "chip8.hpp"
#include "components/renderer.hpp"
#include "debugger/debugWidget.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <span>
#include <string>
#include <utility>

namespace chip8::front
{
    class ConfigurationWidget : public DebugWidget
    {
    public:
        ConfigurationWidget(core::Chip8& chip8, Renderer& renderer);
        void render() override;

    private:
        std::reference_wrapper<core::Chip8> chip8;
        std::reference_wrapper<Renderer> renderer;

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

        std::array<float, 3> foregroundColor{};
        std::array<float, 3> backgroundColor{};

        std::uint32_t arrayToHexColor(std::span<const float, 3> array) const;
        std::array<float, 3> hexToArrayColor(std::uint32_t color) const;
    };
}