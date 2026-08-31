#include "configurationWidget.hpp"
#include "chip8.hpp"
#include "peripherals/display.hpp"
#include "processor/quirks.hpp"
#include <imgui.h>

namespace chip8::front
{
    ConfigurationWidget::ConfigurationWidget(core::Chip8& chip8)
        : chip8(chip8),
        resolutionWidth(chip8.getDisplay().getWidth()),
        resolutionHeight(chip8.getDisplay().getHeight()) { }

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

            ImGui::Text("When running ROMs which do not support resolution modes please default to Hires");
            ImGui::Text("When running games which support resolution modes this should be set to Lores on start");
            ImGui::Text("Changing this setting when the game is running may result in undefined behaviour");
        }

        ImGui::End();
    }
}