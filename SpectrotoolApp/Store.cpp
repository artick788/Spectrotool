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

    void Store::loadProject(const DataTableDesc& desc){
        loader([this, desc] {
            m_Table = createUP<DataTable>();
            const ParserMessages msgs = parseDataTable(desc, *m_Table);
            logParserMessages(msgs);
        });
    }

    void Store::loadProjectStproj(const fs::path &stprojPath) {
        loader([this, stprojPath] {
            m_Table = createUP<DataTable>();
            std::ifstream file(stprojPath);
            nlohmann::json json;
            file >> json;
            m_Table->fromJson(json);
        });
    }

    void Store::exportJson(const fs::path& filePath) const {

    }

    void Store::exportExcel(const fs::path& filePath) {

    }

    void Store::addSampleList(const SampleListDesc &desc) {
        loader([this, desc] {
            SampleList list;
            const ParserMessages msgs = parseSampleList(desc, list);
            logParserMessages(msgs);
            m_Table->setSampleList(list);
        });
    }

    bool Store::isProjectLoading() const {
        return m_IsProjectLoading;
    }

    const UP<DataTable>& Store::getDataTable() const {
        return m_Table;
    }

    void Store::logParserMessages(const ParserMessages &msgs) {
        for (const auto&[severity, message, address, sheet]: msgs) {
            m_ErrorMsg += "[" + severity + "] [" + sheet + "] [" + address + "]: " + message + "\n";
        }
    }


}