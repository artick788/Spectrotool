#pragma once

#include "SpectrotoolAppInclude.hpp"
#include "Worker.hpp"

namespace Spectrotool{

    struct ProjectDesc{
        MassSpecFileDesc massSpecFileDesc;
        SampleListFileDesc sampleListFileDesc;
    };

    class Project{
    public:
        Project(Worker& worker, const fs::path& stprojPath);

        explicit Project(Worker& worker, const ProjectDesc& desc);

        ~Project() = default;

        bool isProjectLoading() const { return m_ProjectLoading; }

        const ProjectDesc& getDesc() const { return m_Desc; }

        const UP<MassSpecFile>& getMassSpecFile() const { return m_MassSpecFile; }

        void exportJson(const fs::path& path);

        void exportExcel(const MassSpecFileExportDesc& desc);

    private:
        ProjectDesc m_Desc;
        bool m_ProjectLoading = false;
        Worker& m_Worker;

        UP<MassSpecFile> m_MassSpecFile = nullptr;

    };

}