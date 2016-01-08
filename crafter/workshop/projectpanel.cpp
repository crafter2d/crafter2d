#include "projectpanel.h"
#include "ui_projectpanel.h"

#include <QVariant>

#include "project/projecttreeobjectitem.h"
#include "script/scriptfile.h"
#include "world/tileworld.h"
#include "world/tileset.h"

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
        mpProject = pproject;

        mpProjectModel->setProject(pproject);
    }
}

#include "newtilesetdialog.h"

void ProjectPanel::on_treeProject_activated(const QModelIndex &index)
{
    QVariant data = mpProjectModel->resourceData(index);
    if ( !data.isValid() )
        return;

    if ( data.canConvert<ResourceHandle>() )
    {
        ResourceHandle& handle = data.value<ResourceHandle>();
        switch ( handle->getType() )
        {
            case Resource::eWorld:
                getMainWindow().showWorld(static_cast<TileWorld&>(*handle));
                break;
            case Resource::eScript:
                getMainWindow().showScript(static_cast<ScriptFile&>(*handle));
                break;
            case Resource::eTileSet:
                NewTileSetDialog::edit(static_cast<TileSet&>(*handle));
                break;
        }
    }
}
