#include "spritePreviewWidget.hpp"
#include "memory/memory.hpp"
#include "processor/processor.hpp"
#include <cstdint>
#include <imgui.h>

namespace chip8::front
{
    SpritePreviewWidget::SpritePreviewWidget(const core::Processor& processor, const core::Memory& memory)
        : processor(processor),
        memory(memory) { }

    void SpritePreviewWidget::render()
    {
        ImGui::Begin("Sprite Preview");

        ImGui::Checkbox("Hires Mode", &hires);
        
        if (hires)
        {
            for (int i = 0; i < 16; ++i)
            {
                std::uint16_t first_byte = static_cast<std::uint16_t>(memory.get().read(processor.get().getRegisterI() + (i * 2)));
                std::uint16_t second_byte = static_cast<std::uint16_t>(memory.get().read(processor.get().getRegisterI() + (i * 2) + 1));
                std::uint16_t row = (first_byte << 8) + second_byte;

                ImGui::Text("%s", getSpriteRow(row).c_str());
            }
        }
        else
        {
            ImGui::SameLine();
            ImGui::SliderInt("Height", &height, 0, 15);

            for (int i = 0; i < height; ++i)
                ImGui::Text("%s", getSpriteRow(memory.get().read(processor.get().getRegisterI() + i)).c_str());
        }
    
        ImGui::End();
    }
}