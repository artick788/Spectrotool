#include "MenuPanel.hpp"

namespace Spectrotool {

    MenuPanel::MenuPanel(UP<SyriusWindow>& window, ResourceView<Context>& context, UP<Store>& store):
    Panel(window, context, store) {
    }

    void MenuPanel::render(WindowSize &size) {
        ImGui::SetNextWindowPos({0.0f, 0.0f});
        ImGui::SetNextWindowSize({static_cast<float>(m_Window->getWidth()), 18.0f});

        ImGui::Begin("MenuPanel", nullptr, ImGuiWindowFlags_NoTitleBar |
                                                          ImGuiWindowFlags_MenuBar |
                                                          ImGuiWindowFlags_NoResize |
                                                          ImGuiWindowFlags_NoBackground);

        if (ImGui::BeginMenuBar()) {
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }


}
