#pragma once

#include "SpectrotoolAppInclude.hpp"
#include "Worker.hpp"

namespace Spectrotool{

    struct ProjectDesc{
        MassSpecFileDesc m_MassSpecFileDesc;
        fs::path m_MassSpecFilePath;
        fs::path m_SampleListFilePath;
    };

    class Project{
    public:
        explicit Project(Worker& worker, const ProjectDesc& desc);

        ~Project() = default;

        bool isProjectLoading() const { return m_ProjectLoading; }

        const ProjectDesc& getDesc() const { return m_Desc; }

    private:
        ProjectDesc m_Desc;
        bool m_ProjectLoading = false;
        Worker& m_Worker;

        UP<MassSpecFile> m_MassSpecFile = nullptr;

    };

}