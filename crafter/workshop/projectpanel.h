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
    virtual ~ProjectPanel();

protected:

  // overrides
    void contextMenuEvent(QContextMenuEvent *event) override;

  // notifications
    virtual void worldActivated(TileWorld* pworld) override;

private slots:
    void on_addExisting_triggered();
    void on_treeProject_activated(const QModelIndex &index);
    void on_treeProject_clicked(const QModelIndex& index);
    void on_projectChanged(Project *pproject);

private:
 // operations
    void createContextMenu();

 // data
    Ui::ProjectPanel *ui;
    Project*        mpProject;
    ProjectModel*   mpProjectModel;
};

#endif // PROJECTPANEL_H
