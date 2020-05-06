#include "projectpanel.h"
#include "ui_projectpanel.h"

#include <QAction>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMenu>
#include <QVariant>

#include "entity/entity.h"
#include "project/projecttreeobjectitem.h"
#include "script/scriptfile.h"
#include "world/tileworld.h"
#include "world/tileset.h"

#include "newentitydialog.h"
#include "newtilesetdialog.h"
#include "mainwindow.h"
#include "projectmodel.h"
#include "projectsettingsdialog.h"
#include "project.h"
#include "tileviewwindow.h"

ProjectPanel::ProjectPanel(MainWindow& mainwindow) :
    DockPanel(mainwindow),
    ui(new Ui::ProjectPanel),
    mpProject(nullptr),
    mpProjectModel(nullptr)
{
    ui->setupUi(this);

    mpProjectModel = new ProjectModel();
    ui->treeProject->setModel(mpProjectModel);

    connect(&mainwindow, SIGNAL(projectChanged(Project*)), SLOT(on_projectChanged(Project*)));
    connect(ui->actionAdd_Existing, SIGNAL(triggered()), SLOT(on_addExisting_triggered()));
    connect(ui->actionSettings, SIGNAL(triggered()), SLOT(on_settings_triggered()));
}

ProjectPanel::~ProjectPanel()
{
    delete ui;
}

// - Overrides

void ProjectPanel::contextMenuEvent(QContextMenuEvent *event)
{
    if ( mpProject )
    {
        QMenu menu(this);
        menu.addAction(ui->actionAdd_Existing);
        menu.addAction(ui->actionSettings);
        menu.exec(event->globalPos());
    }
}

// - Notifications

void ProjectPanel::worldActivated(TileWorld* /* pworld */)
{
    // not really relevant here
}

// - Slots

void ProjectPanel::on_addExisting_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Existing File"), mpProject->getScriptPath(), tr("Script files (*.as)"));
    if  ( file.length() > 0 )
    {
        QFileInfo info(file);

        QString resourcepath = QString("scripts") + QDir::separator() + mpProject->getName() + QDir::separator() + info.fileName();
        QString filepath = mpProject->getBasePath() + QDir::separator() + resourcepath;
        ScriptFile* pscript = new ScriptFile(filepath);
        pscript->setResourceName(resourcepath);

        mpProject->addScript(pscript);
    }
}

void ProjectPanel::on_settings_triggered()
{
    ProjectSettingsDialog dlg(QApplication::desktop());
    dlg.setModal(true);
    dlg.exec();
}

void ProjectPanel::on_projectChanged(Project *pproject)
{
    if ( mpProject != pproject )
    {
        mpProject = pproject;

        mpProjectModel->setProject(pproject);
    }
}

void ProjectPanel::on_treeProject_clicked(const QModelIndex& index)
{
    QVariant data = mpProjectModel->resourceData(index);
    if ( data.isValid() && data.canConvert<ResourceHandle>() )
    {
        ResourceHandle handle = data.value<ResourceHandle>();
        if ( handle->getType() == Resource::eObject )
        {
            getMainWindow().setSelectedEntity(&static_cast<Entity&>(*handle));
        }
    }
}

void ProjectPanel::on_treeProject_activated(const QModelIndex &index)
{
    QVariant data = mpProjectModel->resourceData(index);
    if ( !data.isValid() )
        return;

    if ( data.canConvert<ResourceHandle>() )
    {
        ResourceHandle handle = data.value<ResourceHandle>();
        switch ( handle->getType() )
        {
            case Resource::eObject:
                NewEntityDialog::edit(static_cast<Entity&>(*handle));
                break;
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
