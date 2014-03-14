#include "projectpanel.h"
#include "ui_projectpanel.h"

#include <QVariant>

#include "project/projecttreeworlditem.h"
#include "script/scripthandle.h"

#include "mainwindow.h"
#include "projectmodel.h"
#include "project.h"
#include "tileviewwindow.h"
#include "tileworld.h"

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
        mpProject = pproject;

        mpProjectModel->setProject(pproject);
    }
}

void ProjectPanel::on_treeProject_activated(const QModelIndex &index)
{
    QVariant data = mpProjectModel->resourceData(index);
    if ( !data.isValid() )
        return;

    if ( data.canConvert<TileWorldHandle>() )
    {
        TileWorldHandle& handle = data.value<TileWorldHandle>();
        TileViewWindow* pwindow = getMainWindow().findWindow(*handle);
        if ( pwindow != NULL )
        {
            pwindow->activateWindow();
        }
        else
        {
            getMainWindow().addWindow(*handle);
        }
    }
    else if ( data.canConvert<ScriptHandle>() )
    {
        ScriptHandle& script = data.value<ScriptHandle>();
        getMainWindow().addWindow(*script);
    }
}
