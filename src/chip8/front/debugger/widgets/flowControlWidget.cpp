#include "flowControlWidget.hpp"
#include "application.hpp"
#include <imgui.h>

namespace chip8::front
{
    FlowControlWidget::FlowControlWidget(Application& application, bool horizontal)
        : application(application),
        horizontal(horizontal) { }

    void FlowControlWidget::render()
    {
        ImGui::Begin("Flow Control");

        ImGui::Text("%s", application.get().isRunning() ? "Running" : "Paused");

        if (ImGui::Button("Step"))
            application.get().chipStep();

        if (horizontal)
            ImGui::SameLine();

        if (ImGui::Button("Continue"))
            application.get().setRunning(true);

        if (horizontal)
            ImGui::SameLine();

        if (ImGui::Button("Pause"))
            application.get().setRunning(false);

        if (horizontal)
            ImGui::SameLine();

        if (ImGui::Button("Reset"))
            application.get().reset();

        ImGui::End();
    }
}