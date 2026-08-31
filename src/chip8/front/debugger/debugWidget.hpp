#pragma once

namespace chip8::front
{
    class DebugWidget
    {
    public:
        DebugWidget() = default;
        virtual ~DebugWidget() = default;

        DebugWidget(const DebugWidget&) = delete;
        DebugWidget& operator=(const DebugWidget&) = delete;
        DebugWidget(DebugWidget&&) = delete;
        DebugWidget& operator=(DebugWidget&&) = delete;

        virtual void render() = 0;
    };
}