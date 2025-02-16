#include "MenuPanel.hpp"

#include <fstream>

namespace Spectrotool {

    MenuPanel::MenuPanel(UP<SyriusWindow>& window, ResourceView<Context>& context, UP<Store>& store):
    Panel(window, context, store) {
    }

    void MenuPanel::render(WindowSize &size) {
        ImGui::SetNextWindowPos({size.x, size.y});
        ImGui::SetNextWindowSize({size.width, size.height});

        ImGui::Begin("MenuPanel", nullptr, ImGuiWindowFlags_NoTitleBar |
                                                          ImGuiWindowFlags_MenuBar |
                                                          ImGuiWindowFlags_NoResize |
                                                          ImGuiWindowFlags_NoBackground);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    const auto stprojPath = m_Window->openFileDialog("*.stproj");
                    if (!stprojPath.empty()) {
                        m_Store->loadProjectStproj(stprojPath);
                    }
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    if (m_Store->getProject() != nullptr) {
                        auto savePath = m_Window->saveFileDialog("Untitled", "Spectrotool Project (*.stproj)\0*.stproj\0");
                        savePath += ".stproj";
                        if (!savePath.empty()) {
                            const auto json = m_Store->getProject()->getMassSpecFile()->toJson();
                            std::ofstream file(savePath);
                            file << json.dump(4);
                            file.close();
                        }
                    }
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("About")) {
                ImGui::MenuItem("Version: 0.1", nullptr, false, false);

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }


}
