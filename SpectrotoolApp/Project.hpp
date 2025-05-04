#pragma once

#include "SpectrotoolAppInclude.hpp"
#include "Worker.hpp"

namespace Spectrotool{

    struct ProjectDesc{
        MassSpecFileDesc massSpecFileDesc;
        SampleListFileDesc sampleListFileDesc;
        CorrectionFactorDesc correctionFactorDesc;
    };

    class Project{
    public:
        explicit Project(const fs::path& stprojPath);

        explicit Project(const ProjectDesc& desc);

        ~Project() = default;

        const ProjectDesc& getDesc() const { return m_Desc; }

        const UP<MassSpecFile>& getMassSpecFile() const { return m_MassSpecFile; }

        void exportJson(const fs::path& path) const;

        void exportExcel(const MassSpecFileExportDesc& desc);

    private:
        ProjectDesc m_Desc;

        UP<MassSpecFile> m_MassSpecFile = nullptr;

    };

}