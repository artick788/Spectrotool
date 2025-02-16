#include "ProjectPanel.hpp"

namespace Spectrotool{

    ProjectPanel::ProjectPanel(UP<SyriusWindow> &window, ResourceView<Context> &context, UP<Store> &store) :
    Panel(window, context, store) {

    }

    void ProjectPanel::render(WindowSize &size) {
        ImGui::SetNextWindowPos({size.x, size.y});
        ImGui::SetNextWindowSize({size.width, size.height});
        ImGui::Begin("Compounds", nullptr, ImGuiWindowFlags_NoTitleBar |
                                           ImGuiWindowFlags_NoResize);

        if (m_Store->getProject() != nullptr){
            renderProject(m_Store->getProject());
        }

        ImGui::End();
    }

    void ProjectPanel::renderProject(const UP<Project> &project){
        if (project->isProjectLoading()) {
            ImGui::Text("Loading project...");
            return;
        }
        const auto& compounds = project->getMassSpecFile()->getCompounds();
        if (ImGui::BeginTabBar("CompoundList")){
            for (const auto & compound : compounds){
                if (ImGui::BeginTabItem(compound.getName().c_str())){
                    ImGui::Text("Name: %s", compound.getName().c_str());
                    renderCompound(compound);
                    ImGui::EndTabItem();
                }

            }
            ImGui::EndTabBar();
        }

    }

    void ProjectPanel::renderCompound(const Compound &compound) {
        ImGui::Columns(6, "CompoundColumns");
        ImGui::Separator();
        // First the header
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("RT"); ImGui::NextColumn();
        ImGui::Text("Area"); ImGui::NextColumn();
        ImGui::Text("IS Area"); ImGui::NextColumn();
        ImGui::Text("S/N"); ImGui::NextColumn();
        ImGui::Separator();

        for (const auto& value: compound.getValues()){
            ImGui::Text("%s", value.name.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", value.id.c_str()); ImGui::NextColumn();
            ImGui::Text("%f", value.rt); ImGui::NextColumn();
            ImGui::Text("%f", value.area); ImGui::NextColumn();
            ImGui::Text("%f", value.isArea); ImGui::NextColumn();
            ImGui::Text("%f", value.sDivN); ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }
}
