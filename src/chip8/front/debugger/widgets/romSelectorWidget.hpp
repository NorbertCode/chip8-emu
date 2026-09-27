#pragma once
#include "application.hpp"
#include "debugger/debugWidget.hpp"
#include <filesystem>

namespace chip8::front
{
    class RomSelectorWidget : public DebugWidget
    {
    public:
        RomSelectorWidget(Application& application, const std::filesystem::path& romsPath);
        void render() override;

    private:
        Application& application;
        const std::filesystem::path& romsPath;

        std::vector<std::filesystem::path> roms;

        void refreshRoms();
    };
}