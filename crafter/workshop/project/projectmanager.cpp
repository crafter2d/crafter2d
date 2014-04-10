#include "projectmanager.h"

// - Statics

ProjectManager ProjectManager::mInstance;

ProjectManager& ProjectManager::getInstance()
{
    return mInstance;
}

ProjectManager::ProjectManager():
    QObject(nullptr),
    mpProject(nullptr)
{
}

// - Operations

Project* ProjectManager::getActiveProject()
{
    return mpProject;
}

void ProjectManager::setActiveProject(Project* pproject)
{
    if ( mpProject != pproject)
    {
        mpProject = pproject;

        emit projectChanged(pproject);
    }
}
