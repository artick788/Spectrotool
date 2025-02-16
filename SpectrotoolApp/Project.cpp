#include "Project.hpp"

namespace Spectrotool{

    Project::Project(Worker &worker, const ProjectDesc &desc):
    m_Worker(worker),
    m_Desc(desc){
        m_ProjectLoading = true;
        m_Worker.addTask([this](){
            m_MassSpecFile = createUP<MassSpecFile>(m_Desc.m_MassSpecFilePath, m_Desc.m_MassSpecFileDesc);
            m_ProjectLoading = false;
        });

    }

}
