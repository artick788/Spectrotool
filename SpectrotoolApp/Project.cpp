#include "Project.hpp"

namespace Spectrotool{

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
