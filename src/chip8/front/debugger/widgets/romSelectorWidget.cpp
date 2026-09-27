#include "romSelectorWidget.hpp"
#include "application.hpp"
#include "loading/resourceLoader.hpp"
#include <filesystem>
#include <imgui.h>

namespace chip8::front
{
    RomSelectorWidget::RomSelectorWidget(Application& application, const std::filesystem::path& romsPath)
        : application(application),
        romsPath(romsPath),
        roms(ResourceLoader::findAllRoms(romsPath)) { }

    void RomSelectorWidget::render()
    {
        ImGui::Begin("ROM Selector");

        if (ImGui::Button("Refresh"))
            roms = ResourceLoader::findAllRoms(romsPath);

        ImGui::SameLine();

        ImGui::Text("%s", romsPath.c_str());

        ImGui::Spacing();

        if (ImGui::BeginChild("ROM Selector Region"))
        {
            int totalRows = static_cast<int>(roms.size());

            ImGuiListClipper clipper;
            clipper.Begin(totalRows);

            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
                {
                    ImGui::PushID(row);

                    if (ImGui::Button("Load"))
                        application.setRom(Rom { roms[row], ResourceLoader::loadRom(roms[row]) });

                    ImGui::SameLine();

                    ImGui::Text("%s", roms[row].filename().c_str());

                    ImGui::PopID();
                }
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }
}