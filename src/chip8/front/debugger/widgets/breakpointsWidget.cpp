#include "breakpointsWidget.hpp"
#include "application.hpp"
#include <imgui.h>

namespace chip8::front
{
    BreakpointsWidget::BreakpointsWidget(Application& application)
        : application(application) { }

    void BreakpointsWidget::render()
    {
        ImGui::Begin("Breakpoints");

        ImU16 step = 1;
        ImU16 stepFast = 16;

        ImGui::InputScalar("Line", ImGuiDataType_U16, &line, &step, &stepFast, "0x%04X", ImGuiInputTextFlags_CharsHexadecimal);

        ImGui::SameLine();

        if (ImGui::Button("Add"))
            application.get().addBreakpoint(line);

        ImGui::SameLine();

        if (ImGui::Button("Remove"))
            application.get().removeBreakpoint(line);

        ImGui::BeginGroup();

        for (std::uint16_t line : application.get().getBreakpoints())
        {
            ImGui::Text("0x%04X", line);
        }

        ImGui::EndGroup();

        ImGui::End();
    }
}