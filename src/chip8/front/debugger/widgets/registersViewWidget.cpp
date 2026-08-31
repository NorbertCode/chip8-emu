#include "registersViewWidget.hpp"
#include "processor/processor.hpp"
#include <cstdio>
#include <imgui.h>

namespace chip8::front
{
    RegistersViewerWidget::RegistersViewerWidget(const core::Processor& processor, bool horizontal)
        : processor(processor),
        horizontal(horizontal) { }

    void RegistersViewerWidget::render()
    {
        ImGui::Begin("Registers Viewer");

        ImGui::Checkbox("Horizontal Layout", &horizontal);
        
        ImGui::SameLine();
        if (horizontal)
            ImGui::SliderFloat("Registers Layout Spacing", &horizontalSpacing, 5.0f, 50.0f);
        else
            ImGui::SliderFloat("Registers Layout Spacing", &verticalSpacing, 5.0f, 50.0f);

        size_t totalRows = processor.get().getRegistersV().size();

        ImGui::BeginGroup();

        for (size_t i = 0; i < totalRows; ++i)
        {
            ImGui::Text("V%02zu: 0x%04X", i, processor.get().getRegistersV()[i]);

            if (horizontal)
                ImGui::SameLine(0.0f, horizontalSpacing);
        }

        ImGui::EndGroup();

        if (!horizontal)
            ImGui::SameLine(0.0f, verticalSpacing);

        ImGui::BeginGroup();

        showRegister("I", processor.get().getRegisterI());
        showRegister("PC", processor.get().getProgramCounter());
        showRegister("SP", processor.get().getStackPointer());
        showRegister("DT", processor.get().getDelayTimer());
        showRegister("ST", processor.get().getSoundTimer());

        ImGui::EndGroup();
        
        ImGui::End();
    }
}