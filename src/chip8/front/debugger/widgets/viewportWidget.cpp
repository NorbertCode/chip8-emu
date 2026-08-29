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

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        ImVec2 displaySize;

        float aspectRatio = viewportSize.x / viewportSize.y;
        float targetAspectRatio = static_cast<float>(renderer.get().getWidth()) / static_cast<float>(renderer.get().getHeight());

        if (aspectRatio > targetAspectRatio)
        {
            displaySize.x = targetAspectRatio * viewportSize.y;
            displaySize.y = viewportSize.y;
        }
        else
        {
            displaySize.x = viewportSize.x;
            displaySize.y = viewportSize.x / targetAspectRatio;
        }

        float offsetX = (viewportSize.x - displaySize.x) * 0.5f;
        float offsetY = (viewportSize.y - displaySize.y) * 0.5f;

        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + offsetX, ImGui::GetCursorPosY() + offsetY));
        
        ImGui::Image(
            (ImTextureID)reinterpret_cast<intptr_t>(&renderer.get().getDisplayTexture()),
            displaySize
        );

        ImGui::End();
    }
}