#include "configurationWidget.hpp"
#include "chip8.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"
#include <imgui.h>

namespace chip8::front
{
    ConfigurationWidget::ConfigurationWidget(core::Chip8& chip8, Renderer& renderer)
        : chip8(chip8),
        renderer(renderer),
        resolutionWidth(chip8.getDisplay().getWidth()),
        resolutionHeight(chip8.getDisplay().getHeight()),
        foregroundColor(hexToArrayColor(renderer.getForegroundColor())),
        backgroundColor(hexToArrayColor(renderer.getBackgroundColor())) { }

    void ConfigurationWidget::render()
    {
        ImGui::Begin("Configuration");

        if (ImGui::CollapsingHeader("Quirks"))
        {
            auto& quirks = chip8.get().getProcessor().getQuirks();

            ImGui::Checkbox("VF Reset", &quirks.vfReset);
            ImGui::Checkbox("Index Increment", &quirks.indexIncrement);
            ImGui::Checkbox("Display Clipping", &quirks.displayClipping);
            ImGui::Checkbox("Vy Shifting", &quirks.vyShifting);
            ImGui::Checkbox("Vx Jumping", &quirks.vxJumping);
            ImGui::Checkbox("Clear on Resolution Mode Change", &quirks.clearOnDisplayModeChange);
            ImGui::Checkbox("VF Collision Counter", &quirks.vfCollisionCounter);
            ImGui::Checkbox("Lores Whole Sprite Scrolling", &quirks.loresWholePixelScrolling);
            ImGui::Checkbox("Wait for VBlank", &quirks.waitForVBlank);

            const core::LoresSpriteHandling currentSpriteHandling = quirks.loresSpriteHandling;
            const std::string& preview = loresSpriteHandlingNames[static_cast<size_t>(currentSpriteHandling)].second;
        
            if (ImGui::BeginCombo("Lores Sprite Handling", preview.c_str()))
            {
                for (const auto& [enumValue, name] : loresSpriteHandlingNames)
                {
                    bool isSelected = currentSpriteHandling == enumValue;

                    if (ImGui::Selectable(name.c_str(), isSelected))
                        quirks.loresSpriteHandling = enumValue;

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
        }

        if (ImGui::CollapsingHeader("Display"))
        {
            if (ImGui::InputInt("Width", &resolutionWidth))
                chip8.get().getDisplay().setWidth(resolutionWidth);

            if (ImGui::InputInt("Height", &resolutionHeight))
                chip8.get().getDisplay().setHeight(resolutionHeight);

            const core::ResolutionMode currentResolutionMode = chip8.get().getDisplay().getResolutionMode();
            const std::string& preview = resolutionModeNames[static_cast<size_t>(currentResolutionMode)].second;
        
            if (ImGui::BeginCombo("Resolution Mode", preview.c_str()))
            {
                for (const auto& [enumValue, name] : resolutionModeNames)
                {
                    bool isSelected = currentResolutionMode == enumValue;

                    if (ImGui::Selectable(name.c_str(), isSelected))
                        chip8.get().getDisplay().setResolutionMode(enumValue);

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
            
                ImGui::EndCombo();
            }

            ImGui::TextWrapped("When running ROMs which do not support resolution modes please default to Hires");
            ImGui::TextWrapped("When running games which support resolution modes this should be set to Lores on start");
            ImGui::TextWrapped("Changing this setting when the game is running may result in undefined behaviour");

            if (ImGui::ColorEdit3("Foreground", foregroundColor.data()))
                renderer.get().setForegroundColor(arrayToHexColor(foregroundColor));

            if (ImGui::ColorEdit3("Background", backgroundColor.data()))
                renderer.get().setBackgroundColor(arrayToHexColor(backgroundColor));
        }   

        ImGui::End();
    }

    std::uint32_t ConfigurationWidget::arrayToHexColor(std::span<const float, 3> array) const
    {
        const std::uint32_t red = static_cast<std::uint32_t>(array[0] * 255);
        const std::uint32_t green = static_cast<std::uint32_t>(array[1] * 255);
        const std::uint32_t blue = static_cast<std::uint32_t>(array[2] * 255);

        return (red << 16) | (green << 8) | blue;
    }

    std::array<float, 3> ConfigurationWidget::hexToArrayColor(std::uint32_t color) const
    {
        const std::uint32_t red = (color & 0xFF0000) >> 16;
        const std::uint32_t green = (color & 0xFF00) >> 8;
        const std::uint32_t blue = color & 0xFF;

        return std::array<float, 3> { 
            static_cast<float>(red) / 255.0f,
            static_cast<float>(green) / 255.0f,
            static_cast<float>(blue) / 255.0f
        };
    }
}