#pragma once

#include <QStringList>

class ProjectSettings
{
public:
    ProjectSettings();
    ProjectSettings(const ProjectSettings& other);

    ProjectSettings& operator=(ProjectSettings&& other);

    const QStringList& getBuildPaths() const { return mBuildPaths; }

    void addBuildPath(const QString& path) {
        mBuildPaths.append(path);
    }
    void addBuildPaths(const QStringList& paths) {
        mBuildPaths.append(paths);
    }
    void updateBuildPath(int index, QString&& path);

private:

    QStringList mBuildPaths;
};
