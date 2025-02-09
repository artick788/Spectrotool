#include "FilePanel.hpp"

namespace Spectrotool {

    FilePanel::FilePanel(UP<SyriusWindow> &window, ResourceView<Context> &context, UP<Store> &store):
    Panel(window, context, store){

    }

    void FilePanel::render(WindowSize &size) {
        renderPanelContent(size);
        if (m_OpenVMM) {
            renderOpenVMM();
        }
    }

    void FilePanel::renderPanelContent(WindowSize &size) {
        ImGui::SetNextWindowPos({size.x, size.y});
        ImGui::SetNextWindowSize({300.0f, m_Window->getHeight() - size.y});
        ImGui::Begin("FilePanel", nullptr, ImGuiWindowFlags_NoTitleBar |
                                             ImGuiWindowFlags_NoResize);

        ImGui::Text("Upload new readings");
        ImGui::Separator();
        ImGui::Text("Method"); ImGui::SameLine();
        if (ImGui::Button("VMM", {50.0f, 20.0f})) {
            m_OpenVMM = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("EPA", {50.0f, 20.0f})) {
            std::cout << "EPA" << std::endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("DYI", {50.0f, 20.0f})) {
            std::cout << "Coming Soon..." << std::endl;
        }

        ImGui::End();
    }

    void FilePanel::renderOpenVMM() {
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
        ImGui::Text(massSpecFile.c_str());
        if (ImGui::Button("Load Sample List file", {200.0f, 20.0f})) {
            sampleListFile = m_Window->openFileDialog(".xlsx");
        }
        ImGui::SameLine();
        ImGui::Text(sampleListFile.c_str());

        ImGui::NewLine();
        if (fs::exists(massSpecFile) && fs::exists(sampleListFile)) {
            if (ImGui::Button("Load", {96.0f, 20.0f})) {
                std::cout << "Loading files..." << std::endl;
            }
            ImGui::SameLine();
        }
        if (ImGui::Button("Close", {96.0f, 20.0f})) {
            m_OpenVMM = false;
        }
        auto sizes = ImGui::GetWindowSize();
        windowWidth = sizes.x;
        windowHeight = sizes.y;
        ImGui::End();

    }




}
