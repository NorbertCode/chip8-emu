#include "configSelectorWidget.hpp"
#include "application.hpp"
#include "loading/configParser.hpp"
#include "loading/resourceLoader.hpp"
#include <algorithm>
#include <filesystem>
#include <imgui.h>

namespace chip8::front
{
    ConfigSelectorWidget::ConfigSelectorWidget(Application& application, const std::filesystem::path& configsPath)
        : application(application),
        configsPath(configsPath)
    {
        refreshConfigs();
    }

    void ConfigSelectorWidget::render()
    {
        ImGui::Begin("Config Selector");

        if (ImGui::Button("Refresh"))
            refreshConfigs();

        ImGui::SameLine();

        ImGui::Text("%s", configsPath.c_str());

        ImGui::Spacing();

        if (ImGui::BeginChild("Config Selector Region"))
        {
            int totalRows = static_cast<int>(configs.size());

            ImGuiListClipper clipper;
            clipper.Begin(totalRows);

            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
                {
                    ImGui::PushID(row);

                    if (ImGui::Button("Load"))
                        application.setConfigs(ConfigParser::parseConfig(ResourceLoader::loadConfig(configs[row])));

                    ImGui::SameLine();

                    ImGui::Text("%s", configs[row].filename().c_str());

                    ImGui::PopID();
                }
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }

    void ConfigSelectorWidget::refreshConfigs()
    {
        configs = ResourceLoader::findAllConfigs(configsPath);
        std::sort(configs.begin(), configs.end());
    }
}