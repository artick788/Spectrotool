#include "OverviewPanel.hpp"

namespace Spectrotool {

    OverviewPanel::OverviewPanel(UP<SyriusWindow> &window, ResourceView<Context> &context, UP<Store> &store):
    Panel(window, context, store){

    }

    void OverviewPanel::render(WindowSize &size) {
        renderPanelContent(size);
        if (m_OpenDataFileSelector) {
            renderDataTableFileSelector();
        }
    }

    void OverviewPanel::renderPanelContent(WindowSize &size) {
        ImGui::SetNextWindowPos({size.x, size.y});
        ImGui::SetNextWindowSize({size.width, size.height});
        ImGui::Begin("FilePanel", nullptr, ImGuiWindowFlags_NoTitleBar |
                                             ImGuiWindowFlags_NoResize);

        if (m_Store->getDataTable() != nullptr){
            renderProjectOverview();
        }
        else {
            ImGui::Text("Upload new readings"); ImGui::SameLine();
            if (ImGui::Button("Open", {50.0f, 20.0f})) {
                m_OpenDataFileSelector = true;
            }
        }

        ImGui::End();
    }

    void OverviewPanel::renderDataTableFileSelector() {
        static float windowWidth = 500.0f;
        static float windowHeight = 300.0f;
        ImGui::SetNextWindowSize({windowWidth, windowHeight});
        ImGui::Begin("Load new data", nullptr, ImGuiWindowFlags_NoCollapse);

        static std::string massSpecFile;
        /*
         * Brakste filter code ever
         * rawe arrays for strings letsgooo
         */
        constexpr size_t maxLength = 256;
        static std::vector<std::array<char, maxLength>> filters;
        static std::size_t filterCount = 1;
        auto reset = [&]() {
            massSpecFile.clear();
            filters.clear();
            filterCount = 1;
        };
        if (ImGui::Button("Load Excel file", {200.0f, 20.0f})) {
           massSpecFile = m_Window->openFileDialog(".xlsx");
        }
        ImGui::SameLine();
        ImGui::Text("%s",   massSpecFile.c_str());

        ImGui::Separator();
        ImGui::Text("Exclude compounds whose name contains one of these filters: ");
        for (size_t i = 0; i < filters.size(); ++i) {
            ImGui::PushID(static_cast<int>(i));
            ImGui::InputText("##item", filters[i].data(), filters[i].size());

            ImGui::PopID();
        }

        if (ImGui::Button("Add Compound Filter")) {
            std::array<char, 256> newFilter{};
            std::snprintf(newFilter.data(), newFilter.size(), "filter%zu", filterCount++);
            filters.push_back(newFilter);
        }

        ImGui::NewLine();
        if (fs::exists(massSpecFile)) {
            if (ImGui::Button("Load", {96.0f, 20.0f})) {
                DataTableDesc desc;
                desc.filePath = massSpecFile;
                for (const auto& filter: filters) {
                    std::string f(filter.data(), strnlen(filter.data(), 256));
                    desc.excludeCompoundFilter.push_back(f);

                }
                m_Store->loadProject(desc);
                m_OpenDataFileSelector = false;
                reset();
            }
            ImGui::SameLine();
        }
        if (ImGui::Button("Close", {96.0f, 20.0f})) {
            m_OpenDataFileSelector = false;
            reset();
        }
        const auto sizes = ImGui::GetWindowSize();
        windowWidth = sizes.x;
        windowHeight = sizes.y;
        ImGui::End();
    }

    void OverviewPanel::renderProjectOverview() const {
        if (m_Store->isProjectLoading()){
            ImGui::Text("Loading project...");
            return;
        }
        ImGui::Text("%d Compounds", m_Store->getDataTable()->getCompounds().size());
        ImGui::Text("%d Samples", m_Store->getDataTable()->getSampleCount());

        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();
        static std::string sampleListFile;
        if (ImGui::Button("Load Sample Info", {150.0f, 20.0f})) {
            sampleListFile = m_Window->openFileDialog(".xlsx");
            if (fs::exists(sampleListFile)) {
                SampleListDesc desc;
                desc.filePath = sampleListFile;
                m_Store->addSampleList(desc);
            }
        }
    }

}
