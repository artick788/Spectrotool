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
        ImGui::SetNextWindowSize({300.0f, m_Window->getHeight() - size.y});
        ImGui::Begin("FilePanel", nullptr, ImGuiWindowFlags_NoTitleBar |
                                             ImGuiWindowFlags_NoResize);

        ImGui::Text("Upload new readings"); ImGui::SameLine();
        if (ImGui::Button("Open", {50.0f, 20.0f})) {
            m_OpenFileSelector = true;
        }
        ImGui::Separator();
        if (m_Store->getProject() != nullptr){
            renderProjectOverview();
        }

        ImGui::End();
    }

    void FilePanel::renderFileSelector() {
        static float windowWidth = 500.0f;
        static float windowHeight = 300.0f;
        ImGui::SetNextWindowSize({windowWidth, windowHeight});
        ImGui::Begin("Load new VMM readings", nullptr, ImGuiWindowFlags_NoCollapse);

        static std::string massSpecFile;
        static std::string sampleListFile;
        if (ImGui::Button("Load Mass Spec file", {200.0f, 20.0f})) {
            massSpecFile = m_Window->openFileDialog(".xlsx");
        }
        ImGui::SameLine();
        ImGui::Text("%s", massSpecFile.c_str());
        if (ImGui::Button("Load Sample List file", {200.0f, 20.0f})) {
            sampleListFile = m_Window->openFileDialog(".xlsx");
        }
        ImGui::SameLine();
        ImGui::Text("%s", sampleListFile.c_str());

        ImGui::NewLine();
        if (fs::exists(massSpecFile) && fs::exists(sampleListFile)) {
            if (ImGui::Button("Load", {96.0f, 20.0f})) {
                ProjectDesc desc;
                desc.m_MassSpecFilePath = massSpecFile;
                desc.m_SampleListFilePath = sampleListFile;
                m_Store->loadProject(desc);
                m_OpenFileSelector = false;
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

    void FilePanel::renderProjectOverview() {
        if (m_Store->isProjectLoading()){
            ImGui::Text("Loading project...");
            return;
        }
        ImGui::Text("Mass Spec file: %ls", m_Store->getProject()->getDesc().m_MassSpecFilePath.c_str());
        ImGui::Text("Sample List file: %ls", m_Store->getProject()->getDesc().m_SampleListFilePath.c_str());


    }




}
