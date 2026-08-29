#pragma once
#include "components/renderer.hpp"
#include "debugger/debugWidget.hpp"
#include <functional>

namespace chip8::front
{
    class ViewportWidget : public DebugWidget
    {
    public:
        ViewportWidget(const Renderer& renderer);
        void render() override;

    private:
        std::reference_wrapper<const Renderer> renderer;
    };
}