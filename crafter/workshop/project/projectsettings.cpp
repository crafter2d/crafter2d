#include "projectsettings.h"

ProjectSettings::ProjectSettings():
    mBuildPaths()
{
}

ProjectSettings::ProjectSettings(const ProjectSettings& other):
    mBuildPaths(other.mBuildPaths)
{
}

ProjectSettings& ProjectSettings::operator=(ProjectSettings&& other)
{
    mBuildPaths = std::move(other.mBuildPaths);
    return *this;
}

void ProjectSettings::updateBuildPath(int index, QString&& path)
{
    if ( index >= 0 && index < mBuildPaths.length() ) {
        mBuildPaths[index] = std::move(path);
    }
}
