#include "demoWidget.hpp"
#include <imgui.h>

namespace chip8::front
{
    void DemoWidget::render()
    {
        ImGui::ShowDemoWindow();
    }
}