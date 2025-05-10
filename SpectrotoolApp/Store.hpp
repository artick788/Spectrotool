#pragma once

#include "SpectrotoolAppInclude.hpp"
#include "Worker.hpp"

namespace Spectrotool {

    class Store {
    public:
        Store();

        ~Store() = default;

        void setErrorMsg(const std::string &msg);

        void renderErrorMsg();

        void loadProject(const DataTableDesc& desc);

        void loadProjectStproj(const fs::path& stprojPath);

        void exportJson(const fs::path& filePath);

        void exportExcel(const fs::path& filePath);

        void setSampleList(const SampleListDesc& desc);

        void setCorrectionFactor(const CorrectionFactorDesc& desc);

        bool isProjectLoading() const;

        const UP<DataTable>& getDataTable() const;

    private:

        template<typename... Args>
        void loader(Args&&... args){
            m_IsProjectLoading = true;
            m_Worker.addTask([this, args...]{
                try {
                    std::invoke(args...);
                } catch (std::exception& e) {
                    setErrorMsg(e.what());
                    m_Table = nullptr;
                }
                m_IsProjectLoading = false;
            });
        }

        void logParserMessages(const ParserMessages& msgs);

    private:
        UP<DataTable> m_Table = nullptr;
        std::string m_ErrorMsg;
        Worker m_Worker;

        bool m_IsProjectLoading = false;

    };

}