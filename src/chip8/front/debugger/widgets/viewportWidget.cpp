#include "viewportWidget.hpp"
#include <cstdint>
#include <imgui.h>

namespace chip8::front
{
    ViewportWidget::ViewportWidget(const Renderer& renderer)
        : renderer(renderer) { }

    void ViewportWidget::render()
    {
        ImGui::Begin("Viewport");

        ImGui::Image(
            (ImTextureID)reinterpret_cast<intptr_t>(&renderer.get().getDisplayTexture()),
            ImGui::GetContentRegionAvail()
        );

        ImGui::End();
    }
}