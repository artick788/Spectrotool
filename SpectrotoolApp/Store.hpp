#pragma once

#include "SpectrotoolAppInclude.hpp"
#include "Worker.hpp"
#include "Project.hpp"

namespace Spectrotool {

    class Store {
    public:
        Store();

        ~Store() = default;

        void setErrorMsg(const std::string &msg);

        void renderErrorMsg();

        void loadProject(const ProjectDesc& desc);

        void loadProjectStproj(const fs::path& stprojPath);

        void exportJson(const fs::path& path) const;

        void exportExcel(const MassSpecFileExportDesc& desc);

        bool isProjectLoading() const;

        const UP<Project>& getProject() const;

    private:

        template<typename... Args>
        void loader(Args&&... args){
            m_IsProjectLoading = true;
            m_Worker.addTask([this, args...]{
                try {
                    std::invoke(args...);
                } catch (std::exception& e) {
                    setErrorMsg(e.what());
                    m_Project = nullptr;
                }
                m_IsProjectLoading = false;
            });
        }

    private:
        UP<Project> m_Project = nullptr;
        std::string m_ErrorMsg;
        Worker m_Worker;

        bool m_IsProjectLoading = false;

    };

}