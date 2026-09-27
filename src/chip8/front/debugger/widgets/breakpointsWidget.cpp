#include "breakpointsWidget.hpp"
#include "debugger/debugger.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>

namespace chip8::front
{
    BreakpointsWidget::BreakpointsWidget(Debugger& debugger)
        : debugger(debugger) { }

    void BreakpointsWidget::render()
    {
        ImGui::Begin("Breakpoints");

        if (ImGui::InputText("Line (Hex)", &input, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsNoBlank) && !input.empty())
            line = std::stoul(input, nullptr, 16);

        if (ImGui::Button("Add"))
            debugger.get().addBreakpoint(line);

        ImGui::SameLine();

        if (ImGui::Button("Remove"))
            debugger.get().removeBreakpoint(line);

        ImGui::BeginGroup();

        for (std::uint16_t line : debugger.get().getBreakpoints())
        {
            ImGui::Text("0x%04X", line);
        }

        ImGui::EndGroup();

        ImGui::End();
    }
}