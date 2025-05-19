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
                if (m_Store->getDataTable() != nullptr) {
                    renderSaveMenuItem();
                    renderExportExcelMenuItem();
                    renderClearMenuItem();
                }
                else {
                    renderOpenMenuItem();
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("About")) {
                ImGui::MenuItem("Version: 0.1", nullptr, false, false);

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        renderClearConfirmation();
        ImGui::End();
    }

    void MenuPanel::renderOpenMenuItem() const {
        if (ImGui::MenuItem("Open", "Ctrl+O")) {
            const auto stprojPath = m_Window->openFileDialog("*.stproj");
            if (!stprojPath.empty()) {
                m_Store->loadProjectStproj(stprojPath);
            }
        }
    }

    void MenuPanel::renderSaveMenuItem() const {
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            auto savePath = m_Window->saveFileDialog("Untitled", "Spectrotool Project (*.stproj)\0*.stproj\0");
            if (savePath.find(".stproj") == std::string::npos) {
                savePath += ".stproj";
            }
            savePath += ".stproj";
            if (!savePath.empty()) {
                m_Store->exportJson(savePath);
            }
        }
    }

    void MenuPanel::renderExportExcelMenuItem() const {
        if (ImGui::MenuItem("Export Excel", "Ctrl+E")) {
            auto savePath = m_Window->saveFileDialog("Untitled", "Excel (*.xlsx)\0*.xlsx\0");
            if (savePath.find(".xlsx") == std::string::npos) {
                savePath += ".xlsx";
            }
            if (!savePath.empty()) {
                m_Store->exportExcel(savePath);
            }
        }
    }

    void MenuPanel::renderClearMenuItem() {
        if (ImGui::MenuItem("Clear")) {
            m_ShowClearConfirmation = true;
        }
    }

    void MenuPanel::renderClearConfirmation() {
        if (m_ShowClearConfirmation) {
            ImGui::OpenPopup("Clear?");
            ImGui::SetNextWindowSize(ImVec2(400, 150));
            if (ImGui::BeginPopupModal("Clear?")) {
                ImGui::TextWrapped("Are you sure you want to clear the current workspace? All unsaved changes will be lost!");

                if (ImGui::Button("Cancel")) {
                    m_ShowClearConfirmation = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear!")) {
                    m_Store->clear();
                    m_ShowClearConfirmation = false;
                }

                ImGui::EndPopup();
            }
        }
    }

}
