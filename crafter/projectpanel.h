#ifndef PROJECTPANEL_H
#define PROJECTPANEL_H

#include "dockpanel.h"

namespace Ui {
class ProjectPanel;
}

class Project;
class ProjectModel;

class ProjectPanel : public DockPanel
{
    Q_OBJECT

public:
    explicit ProjectPanel(MainWindow &mainwindow);
    ~ProjectPanel();

protected:

  // notifications
    virtual void worldActivated(TileWorld* pworld) override;

private slots:
    void on_treeProject_activated(const QModelIndex &index);
    void on_projectChanged(Project *pproject);

private:
 // data
    Ui::ProjectPanel *ui;
    Project*        mpProject;
    ProjectModel*   mpProjectModel;
};

#endif // PROJECTPANEL_H
