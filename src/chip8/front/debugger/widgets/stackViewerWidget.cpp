#include "stackViewerWidget.hpp"
#include "processor/processor.hpp"
#include <imgui.h>

namespace chip8::front
{
    StackViewerWidget::StackViewerWidget(const core::Processor& processor, bool horizontal)
        : processor(processor),
        horizontal(horizontal) { }

    void StackViewerWidget::render()
    {
        ImGui::Begin("Stack Viewer");

        ImGui::Checkbox("Horizontal Layout", &horizontal);
        
        if (horizontal)
        {
            ImGui::SameLine();
            ImGui::SliderFloat("Stack Layout Spacing", &spacing, 5.0f, 50.0f);
        }

        if (ImGui::BeginChild("Stack View Scroll Region"))
        {
            size_t totalRows = processor.get().getStack().size();

            for (size_t i = 0; i < totalRows; ++i)
            {
                ImGui::Text("%02zu: 0x%04X", i, processor.get().getStack()[i]);

                if (horizontal)
                    ImGui::SameLine(0.0f, spacing);
            }
        }
        ImGui::EndChild();
        
        ImGui::End();
    }
}