#include "Project.hpp"

#include <fstream>

namespace Spectrotool{

    Project::Project(Worker &worker, const fs::path &stprojPath):
    m_Worker(worker) {
        m_ProjectLoading = true;
        m_Worker.addTask([this, stprojPath]() {
            std::ifstream file(stprojPath);
            nlohmann::json json;
            file >> json;
            m_MassSpecFile = createUP<MassSpecFile>();
            m_MassSpecFile->fromJson(json);
            m_ProjectLoading = false;
        });
    }

    Project::Project(Worker &worker, const ProjectDesc &desc):
    m_Worker(worker),
    m_Desc(desc){
        m_ProjectLoading = true;
        m_Worker.addTask([this](){
            m_MassSpecFile = createUP<MassSpecFile>(m_Desc.massSpecFileDesc);
            const SampleListFile sampleListFile(m_Desc.sampleListFileDesc);
            m_MassSpecFile->setSampleInfo(sampleListFile);
            m_ProjectLoading = false;
        });

    }

}
