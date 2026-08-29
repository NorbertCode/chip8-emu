#include "stackViewerWidget.hpp"
#include "processor/processor.hpp"
#include <imgui.h>

namespace chip8::front
{
    StackViewerWidget::StackViewerWidget(const core::Processor& processor)
        : processor(processor) { }

    void StackViewerWidget::render()
    {
        ImGui::Begin("Stack Viewer");

        int totalRows = static_cast<int>(processor.get().getStack().size());

        ImGuiListClipper clipper;
        clipper.Begin(totalRows);

        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
                ImGui::Text("%02d: 0x%04X", i, processor.get().getStack()[i]);
        }
        
        ImGui::End();
    }
}