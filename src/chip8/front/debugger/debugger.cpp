#include "debugger.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <utility>

namespace chip8::front
{
    Debugger::Debugger()
        : widgets(0),
        isValid(false) { }

    Debugger::Debugger(SDL_Window& window, SDL_Renderer& renderer, std::vector<std::unique_ptr<DebugWidget>> widgets)
        : widgets(std::move(widgets)),
        isValid(true)
    {
        ImGui::CreateContext();

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplSDL2_InitForSDLRenderer(&window, &renderer);
        ImGui_ImplSDLRenderer2_Init(&renderer);
    }

    Debugger::~Debugger()
    {
        if (!isValid)
            return;

        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    Debugger::Debugger(Debugger&& other) noexcept
        : widgets(std::move(other.widgets)),
        isValid(std::exchange(other.isValid, false)),
        layoutInitialized(std::exchange(other.layoutInitialized, false)) { }

    Debugger& Debugger::operator=(Debugger&& other) noexcept
    {
        if (this != &other)
        {
            widgets = std::move(other.widgets);
            isValid = other.isValid;
            layoutInitialized = other.layoutInitialized;

            other.isValid = false;
        }

        return *this;
    }

    void Debugger::processEvent(const SDL_Event& event)
    {
        if (isValid)
            ImGui_ImplSDL2_ProcessEvent(&event);
    }

    void Debugger::draw()
    {
        if (!isValid)
            return;

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGuiID dockSpaceId = ImGui::GetID("DockSpace");

        if (!layoutInitialized)
        {
            layoutInitialized = true;

            ImGui::DockBuilderRemoveNode(dockSpaceId);

            ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetMainViewport()->Size);

            ImGuiID dockMainId = dockSpaceId;
            ImGuiID dockRightId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.25f, nullptr, &dockMainId);
            ImGuiID dockBottomRightId = ImGui::DockBuilderSplitNode(dockRightId, ImGuiDir_Down, 0.25f, nullptr, &dockRightId);
        
            ImGui::DockBuilderDockWindow("Disassembly", dockRightId);
            ImGui::DockBuilderDockWindow("Memory Viewer", dockRightId);
            ImGui::DockBuilderDockWindow("Stack Viewer", dockBottomRightId);
            ImGui::DockBuilderDockWindow("Viewport", dockMainId);
            ImGui::DockBuilderDockWindow("Registers Viewer", dockBottomRightId);
            ImGui::DockBuilderDockWindow("Sprite Preview", dockBottomRightId);

            ImGui::DockBuilderFinish(dockSpaceId);
        }

        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | 
            ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("DockSpace", nullptr, flags);

        ImGui::DockSpace(dockSpaceId);

        ImGui::End();

        for (auto& widget : widgets)
            widget->render();
    }

    void Debugger::render(SDL_Renderer& renderer)
    {
        if (!isValid)
            return;

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), &renderer);
    }
}