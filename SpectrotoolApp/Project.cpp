#include "Project.hpp"

#include <fstream>

namespace Spectrotool{

    Project::Project(const fs::path &stprojPath){
        std::ifstream file(stprojPath);
        nlohmann::json json;
        file >> json;
        m_MassSpecFile = createUP<MassSpecFile>();
        m_MassSpecFile->fromJson(json);

    }

    Project::Project(const ProjectDesc &desc):
    m_Desc(desc){
        m_MassSpecFile = createUP<MassSpecFile>(m_Desc.massSpecFileDesc);

        // Load sample info
        const SampleListFile sampleListFile(m_Desc.sampleListFileDesc);
        m_MassSpecFile->setSampleInfo(sampleListFile);

        // do correction
        const CorrectionFactor factor(desc.correctionFactorDesc);
    }

    void Project::exportJson(const fs::path &path) const {
        const auto json = m_MassSpecFile->toJson();
        std::ofstream file(path);
        file << json.dump(2);
        file.close();
    }

    void Project::exportExcel(const MassSpecFileExportDesc &desc) {
        m_MassSpecFile->exportToExcel(desc);
    }


}
