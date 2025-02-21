#include "Store.hpp"

namespace Spectrotool {

    Store::Store():
    m_Worker("Store") {
    }

    void Store::setErrorMsg(const std::string &msg) {
        m_ErrorMsg = msg;
    }

    void Store::renderErrorMsg() {
        if (m_ErrorMsg.empty()){
            return;
        }
        ImGui::Begin("Error", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("%s", m_ErrorMsg.c_str());
        ImGui::Separator();
        if (ImGui::Button("Close", {150.0f, 20.0f})) {
            m_ErrorMsg.clear();
        }
        ImGui::End();
    }

    void Store::loadProject(const ProjectDesc& desc){
        loader([this, desc] {
            m_Project = createUP<Project>(desc);
        });
    }

    void Store::loadProjectStproj(const fs::path &stprojPath) {
        loader([this, stprojPath] {
            m_Project = createUP<Project>(stprojPath);
        });
    }


    bool Store::isProjectLoading() const {
        return m_IsProjectLoading;
    }

    const UP<Project>& Store::getProject() const {
        return m_Project;
    }

}