#include "FilePanel.hpp"
#include "../Project.hpp"

namespace Spectrotool {

    FilePanel::FilePanel(UP<SyriusWindow> &window, ResourceView<Context> &context, UP<Store> &store):
    Panel(window, context, store){

    }

    void FilePanel::render(WindowSize &size) {
        renderPanelContent(size);
        if (m_OpenFileSelector) {
            renderFileSelector();
        }
    }

    void FilePanel::renderPanelContent(WindowSize &size) {
        ImGui::SetNextWindowPos({size.x, size.y});
        ImGui::SetNextWindowSize({size.width, size.height});
        ImGui::Begin("FilePanel", nullptr, ImGuiWindowFlags_NoTitleBar |
                                             ImGuiWindowFlags_NoResize);

        if (m_Store->getProject() != nullptr){
            renderProjectOverview();
        }
        else {
            ImGui::Text("Upload new readings"); ImGui::SameLine();
            if (ImGui::Button("Open", {50.0f, 20.0f})) {
                m_OpenFileSelector = true;
            }
        }

        ImGui::End();
    }

    void FilePanel::renderFileSelector() {
        static float windowWidth = 500.0f;
        static float windowHeight = 300.0f;
        ImGui::SetNextWindowSize({windowWidth, windowHeight});
        ImGui::Begin("Load new readings", nullptr, ImGuiWindowFlags_NoCollapse);

        static std::string massSpecFile;
        static std::string sampleListFile;
        static std::string correctionFactorFile;
        if (ImGui::Button("Load Mass Spec file", {200.0f, 20.0f})) {
           massSpecFile = m_Window->openFileDialog(".xlsx");
        }
        ImGui::SameLine();
        ImGui::Text("%s",   massSpecFile.c_str());
        if (ImGui::Button("Load Sample List file", {200.0f, 20.0f})) {
            sampleListFile = m_Window->openFileDialog(".xlsx");
        }
        ImGui::SameLine();
        ImGui::Text("%s", sampleListFile.c_str());

        if (ImGui::Button("Load Correction Factor file", {200.0f, 20.0f})) {
            correctionFactorFile = m_Window->openFileDialog(".xlsx");
        }
        ImGui::SameLine();
        ImGui::Text("%s", correctionFactorFile.c_str());

        static std::vector<std::string> correctionFactorMethods = {"VMM", "Wellington", "EPA"};
        static int selectedMethod = -1;
        if (ImGui::BeginCombo("Correction Method",
                              (selectedMethod >= 0 && selectedMethod < correctionFactorMethods.size())
                              ? correctionFactorMethods[selectedMethod].c_str()
                              : "Select...")) {
            for (int i = 0; i < correctionFactorMethods.size(); ++i) {
                bool isSelected = (selectedMethod == i);
                if (ImGui::Selectable(correctionFactorMethods[i].c_str(), isSelected)) {
                    selectedMethod = i;
                }

                // Set the initial focus when opening the combo (optional)
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();
        ImGui::Text("Exclude compounds containing: ");
        ImGui::InputTextWithHint("##excludeFilter", "Filter", &m_ProjectDesc.massSpecFileDesc.excludeFilter);

        ImGui::NewLine();
        if (fs::exists(massSpecFile) && fs::exists(sampleListFile)) {
            if (ImGui::Button("Load", {96.0f, 20.0f})) {
                m_ProjectDesc.massSpecFileDesc.filePath = massSpecFile;
                m_ProjectDesc.sampleListFileDesc.filePath = sampleListFile;
                m_Store->loadProject(m_ProjectDesc);
                m_OpenFileSelector = false;
                massSpecFile.clear();
                sampleListFile.clear();
                m_ProjectDesc.massSpecFileDesc.excludeFilter.clear();

            }
            ImGui::SameLine();
        }
        if (ImGui::Button("Close", {96.0f, 20.0f})) {
            m_OpenFileSelector = false;
        }
        auto sizes = ImGui::GetWindowSize();
        windowWidth = sizes.x;
        windowHeight = sizes.y;
        ImGui::End();
    }

    void FilePanel::renderProjectOverview() const {
        if (m_Store->isProjectLoading()){
            ImGui::Text("Loading project...");
            return;
        }
        ImGui::Text("%d of %d Compounds selected", m_Store->getProject()->getMassSpecFile()->getCompounds().size(),  m_Store->getProject()->getMassSpecFile()->getReadCompoundCount());
        ImGui::Text("%d Samples", m_Store->getProject()->getMassSpecFile()->getSampleCount());
    }




}
