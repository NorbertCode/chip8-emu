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

            const int currentSpriteHandling = static_cast<int>(chip8.get().getProcessor().getQuirks().loresSpriteHandling);
            const std::string& preview = loresSpriteHandlingNames[currentSpriteHandling];
        
            if (ImGui::BeginCombo("Lores Sprite Handling", preview.c_str()))
            {
                for (size_t i = 0; i < loresSpriteHandlingNames.size(); ++i)
                {
                    bool isSelected = currentSpriteHandling == i;

                    if (ImGui::Selectable(loresSpriteHandlingNames[i].c_str(), isSelected))
                        quirks.loresSpriteHandling = static_cast<core::LoresSpriteHandling>(i);

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

            const int currentResolutionMode = static_cast<int>(chip8.get().getDisplay().getResolutionMode());
            const std::string& preview = resolutionModeNames[currentResolutionMode];
        
            if (ImGui::BeginCombo("Resolution Mode", preview.c_str()))
            {
                for (size_t i = 0; i < resolutionModeNames.size(); ++i)
                {
                    bool isSelected = currentResolutionMode == i;

                    if (ImGui::Selectable(resolutionModeNames[i].c_str(), isSelected))
                        chip8.get().getDisplay().setResolutionMode(static_cast<core::ResolutionMode>(i));

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