#pragma once
#include "application.hpp"
#include "debugger/debugWidget.hpp"
#include <filesystem>

namespace chip8::front
{
    class ConfigSelectorWidget : public DebugWidget
    {
    public:
        ConfigSelectorWidget(Application& application, const std::filesystem::path& configsPath);
        void render() override;

    private:
        Application& application;
        const std::filesystem::path& configsPath;

        std::vector<std::filesystem::path> configs;
    };
}