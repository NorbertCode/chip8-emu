#pragma once
#include "debugger/debugWidget.hpp"
#include "processor/processor.hpp"
#include <imgui.h>
#include <concepts>
#include <functional>

namespace chip8::front
{
    class RegistersViewerWidget : public DebugWidget
    {
    public:
        RegistersViewerWidget(const core::Processor& processor, bool horizontal = false);
        void render() override;

    private:
        std::reference_wrapper<const core::Processor> processor;

        bool horizontal = false;
        float horizontalSpacing = 20.0f;
        float verticalSpacing = 20.0f;

        template <std::integral T>
        void showRegister(const std::string& name, T value)
        {
            ImGui::Text("%3s: 0x%04X", name.c_str(), value);

            if (horizontal)
                ImGui::SameLine(0.0f, horizontalSpacing);
        }
    };
}