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
        m_Project = createUP<Project>(m_Worker, desc);
    }

    bool Store::isProjectLoading() const {
        if (m_Project == nullptr){
            return false;
        }
        return m_Project->isProjectLoading();
    }

}