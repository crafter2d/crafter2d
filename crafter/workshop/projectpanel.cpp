#include "projectpanel.h"
#include "ui_projectpanel.h"

#include "mainwindow.h"
#include "projectmodel.h"
#include "project.h"
#include "tileviewwindow.h"

ProjectPanel::ProjectPanel(MainWindow& mainwindow) :
    DockPanel(mainwindow),
    ui(new Ui::ProjectPanel),
    mpProject(NULL),
    mpProjectModel(NULL)
{
    ui->setupUi(this);

    mpProjectModel = new ProjectModel();
    ui->treeProject->setModel(mpProjectModel);

    connect(&mainwindow, SIGNAL(projectChanged(Project*)), SLOT(on_projectChanged(Project*)));
}

ProjectPanel::~ProjectPanel()
{
    delete ui;
}

// - Notifications

void ProjectPanel::worldActivated(TileWorld* /* pworld */)
{
    // not really relevant here
}

// - Slots

void ProjectPanel::on_projectChanged(Project *pproject)
{
    if ( mpProject != pproject )
    {
        delete mpProject;
        mpProject = pproject;

        mpProjectModel->setProject(pproject);
    }
}

void ProjectPanel::on_treeProject_activated(const QModelIndex &index)
{
    if ( index.row() < mpProject->getWorldCount() )
    {
        TileWorld& world = mpProject->getWorld(index.row());

        TileViewWindow* pwindow = getMainWindow().findWindow(&world);
        if ( pwindow != NULL )
        {
            pwindow->activateWindow();
        }
        else
        {
            getMainWindow().addWindow(world);
        }
    }
}
