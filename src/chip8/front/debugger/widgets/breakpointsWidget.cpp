#include "breakpointsWidget.hpp"
#include "application.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>

namespace chip8::front
{
    BreakpointsWidget::BreakpointsWidget(Application& application)
        : application(application) { }

    void BreakpointsWidget::render()
    {
        ImGui::Begin("Breakpoints");

        if (ImGui::InputText("Line (Hex)", &input, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsNoBlank))
            line = std::stoul(input, nullptr, 16);

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