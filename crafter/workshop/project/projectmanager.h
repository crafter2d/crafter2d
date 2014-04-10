#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>

class Project;

class ProjectManager : public QObject
{
    Q_OBJECT

public:
    static ProjectManager& getInstance();

  // operations
    Project* getActiveProject();
    void     setActiveProject(Project *pproject);

signals:
    void projectChanged(Project* pnewproject);

private:
    ProjectManager();

  // static data
    static ProjectManager mInstance;

  // data
    Project* mpProject;
};

#endif // PROJECTMANAGER_H
