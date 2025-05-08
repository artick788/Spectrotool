#include "DataTablePanel.hpp"

namespace Spectrotool{

    DataTablePanel::DataTablePanel(UP<SyriusWindow> &window, ResourceView<Context> &context, UP<Store> &store) :
    Panel(window, context, store) {

    }

    void DataTablePanel::render(WindowSize &size) {
        ImGui::SetNextWindowPos({size.x, size.y});
        ImGui::SetNextWindowSize({size.width, size.height});
        ImGui::Begin("Compounds", nullptr, ImGuiWindowFlags_NoTitleBar |
                                           ImGuiWindowFlags_NoResize);

        if (m_Store->isProjectLoading()) {
            ImGui::Text("Loading project...");
        }
        else if (m_Store->getDataTable() != nullptr){
            renderTable(m_Store->getDataTable());
        }
        else {
            ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, "No project loaded! Please load some readings or open an existing project.");
        }

        ImGui::End();
    }

    void DataTablePanel::renderTable(const UP<DataTable> &project){
        const auto& compounds = project->getCompounds();
        if (ImGui::BeginTabBar("CompoundList")){
            for (const auto & compound : compounds){
                if (ImGui::BeginTabItem(compound.getName().c_str())){
                    ImGui::Text("Name: %s", compound.getName().c_str());
                    const auto& values = compound.getValues();
                    if (values.empty()){
                        ImGui::Text("No values found for compound");
                    }
                    else {
                        renderCompound(compound);
                    }
                    ImGui::EndTabItem();
                }

            }
            ImGui::EndTabBar();
        }

    }

    void DataTablePanel::renderCompound(const Compound &compound) {
        ImGui::Columns(12, "CompoundColumns");
        ImGui::Separator();
        // First the header
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("RT"); ImGui::NextColumn();
        ImGui::Text("Area"); ImGui::NextColumn();
        ImGui::Text("IS Area"); ImGui::NextColumn();
        ImGui::Text("S/N"); ImGui::NextColumn();

        ImGui::Text("Weight"); ImGui::NextColumn();
        ImGui::Text("Matrix"); ImGui::NextColumn();

        ImGui::Text("Corrected IS Area"); ImGui::NextColumn();
        ImGui::Text("Uncorrected Concentration (pg/g)"); ImGui::NextColumn();
        ImGui::Text("Uncorrected Concentration (ug/kg)"); ImGui::NextColumn();
        ImGui::Text("Corrected Concentration (ug/kg)"); ImGui::NextColumn();

        ImGui::Separator();

        for (const auto& value: compound.getValues()){
            ImGui::Text("%s", value.name.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", value.id.c_str()); ImGui::NextColumn();
            ImGui::Text("%f", value.rt); ImGui::NextColumn();
            ImGui::Text("%f", value.area); ImGui::NextColumn();
            ImGui::Text("%f", value.isArea); ImGui::NextColumn();
            ImGui::Text("%f", value.sDivN); ImGui::NextColumn();

            ImGui::Text("%f", value.weight); ImGui::NextColumn();
            ImGui::Text("%s", value.matrix.c_str()); ImGui::NextColumn();

            ImGui::Text("%f", value.correctedISArea); ImGui::NextColumn();
            ImGui::Text("%f", value.uncorrectedConcentration_pgg); ImGui::NextColumn();
            ImGui::Text("%f", value.uncorrectedConcentration_microgkg); ImGui::NextColumn();
            ImGui::Text("%f", value.correctedConcentration); ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }
}
