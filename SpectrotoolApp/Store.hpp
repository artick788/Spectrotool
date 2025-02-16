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

        bool isProjectLoading() const;

        const UP<Project>& getProject() const { return m_Project; }

    private:
        UP<Project> m_Project = nullptr;
        std::string m_ErrorMsg;
        Worker m_Worker;

    };

}