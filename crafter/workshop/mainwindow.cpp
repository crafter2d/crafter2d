#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QMdiSubWindow>
#include <QMessageBox>

#include "aboutdialog.h"
#include "newprojectdialog.h"
#include "newlayerdialog.h"
#include "newworlddialog.h"
#include "project.h"
#include "projectpanel.h"
#include "layerpanel.h"
#include "tilespanel.h"
#include "tileview.h"
#include "tileviewwindow.h"
#include "tileworld.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mpProject(NULL),
    mpRecentFileActions(),
    mpLayerPanel(NULL),
    mpTilesPanel(NULL)
{
    ui->setupUi(this);

    QActionGroup* pgroup = new QActionGroup(this);
    pgroup->addAction(ui->actionEdit_layers);
    pgroup->addAction(ui->actionEdit_bounds);

    restorePanels();
    readSettings();

    setRecentFileActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// - Query

QMdiArea& MainWindow::getMdiArea()
{
    return *ui->centralWidget;
}

TileView* MainWindow::getActiveView()
{
    QMdiSubWindow* pwindow = ui->centralWidget->activeSubWindow();
    if ( pwindow != NULL )
    {
        return &dynamic_cast<TileViewWindow*>(pwindow->widget())->getTileView();
    }
    return NULL;
}

// - Operations

void MainWindow::setProject(Project* pproject)
{
    if ( pproject != mpProject )
    {
        delete mpProject;
        mpProject = pproject;

        emit projectChanged(mpProject);
    }
}

void MainWindow::setRecentFileActions()
{
    mpRecentFileActions[0] = ui->action_recent1;
    mpRecentFileActions[1] = ui->action_recent2;
    mpRecentFileActions[2] = ui->action_recent3;
    mpRecentFileActions[3] = ui->action_recent4;
    mpRecentFileActions[4] = ui->action_recent5;

    for (int index = 0; index < MaxRecentFiles; ++index )
    {
        QAction* paction = mpRecentFileActions[index];
        paction->setVisible(false);

        connect(paction, SIGNAL(triggered()), this, SLOT(on_actionOpenRecentFile_triggered()));
    }

    updateRecentFileList();
}

void MainWindow::updateRecentFileList()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for ( int index = 0; index < numRecentFiles; ++index )
    {
        QString text = tr("&%1 %2").arg(index + 1).arg(strippedName(files[index]));
        mpRecentFileActions[index]->setText(text);
        mpRecentFileActions[index]->setData(files[index]);
        mpRecentFileActions[index]->setVisible(true);
    }

    for ( int index = numRecentFiles; index < MaxRecentFiles; ++index )
        mpRecentFileActions[index]->setVisible(false);
}

void MainWindow::setCurrentFile(const QString& fileName)
{
    setWindowFilePath(fileName);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while ( files.size() > MaxRecentFiles )
    {
        files.removeLast();
    }

    settings.setValue("recentFileList", files);

    updateRecentFileList();
}

QString MainWindow::strippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::loadProject(const QString& fileName)
{
    Project* pproject = new Project();
    if ( !pproject->load(fileName) )
    {
        QMessageBox box;
        box.setText("Could not load the world file.");
        box.exec();
    }
    else
    {
        setProject(pproject);

        setCurrentFile(fileName);
    }
}

void MainWindow::saveProject()
{
    if ( mpProject != NULL )
    {
        mpProject->save();

        setCurrentFile(mpProject->getFileName());
    }
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowstate").toByteArray());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowstate", saveState());
    settings.endGroup();
}

void MainWindow::restorePanels()
{
    mpProjectPanel = new ProjectPanel(*this);
    addDockWidget(Qt::LeftDockWidgetArea, mpProjectPanel);

    mpTilesPanel = new TilesPanel(*this);
    addDockWidget(Qt::RightDockWidgetArea, mpTilesPanel);

    mpLayerPanel = new LayerPanel(*this);
    addDockWidget(Qt::RightDockWidgetArea, mpLayerPanel);
}

void MainWindow::showPanel(DockPanel* ppanel, bool show)
{
    if ( show )
    {
        restoreDockWidget(ppanel);
        ppanel->show();
    }
    else
    {
        ppanel->hide();
    }
}

void MainWindow::addWindow(TileWorld& world)
{
    TileViewWindow* pwindow = new TileViewWindow();
    pwindow->setWorld(world);
    pwindow->show();

    ui->centralWidget->addSubWindow(pwindow);
}

// - Search

TileViewWindow* MainWindow::findWindow(TileWorld* pworld)
{
    QMdiSubWindow* psubwindow = NULL;
    QList<QMdiSubWindow*> subwindows = ui->centralWidget->subWindowList();
    foreach (psubwindow, subwindows)
    {
        TileViewWindow* pwindow = static_cast<TileViewWindow*>(psubwindow->widget());
        TileWorld& world = pwindow->getTileView().getWorld();
        if ( &world == pworld )
        {
            return pwindow;
        }
    }
    return NULL;
}

// - Event handlers

void MainWindow::closeEvent(QCloseEvent *pevent)
{
    writeSettings();

    pevent->accept();
}

// - Signal handlers

void MainWindow::on_actionNew_triggered()
{
    NewProjectDialog dialog;
    int result = dialog.exec();

    if ( result == QDialog::Accepted )
    {
        QString projectname = dialog.getName();
        QDir projectpath(dialog.getPath());
        QString projectfile = projectpath.absoluteFilePath(projectname + ".craft");

        Project* pproject = new Project();
        pproject->setName(projectname);
        pproject->setFileName(projectfile);

        setProject(pproject);
        saveProject();
    }
}

void MainWindow::on_actionSave_triggered()
{
    saveProject();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), "e:/devel", tr("Crafter Project Files (*.craft)"));

    if ( !fileName.isEmpty() )
    {
        loadProject(fileName);
    }
}

void MainWindow::on_actionOpenRecentFile_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if ( action != NULL )
    {
        loadProject(action->data().toString());
    }
}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}

void MainWindow::on_actionEdit_Undo_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        pview->undo();
    }
}

void MainWindow::on_actionEdit_Redo_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        pview->redo();
    }
}

void MainWindow::on_actionEdit_layers_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        pview->setEditMode(TileView::eLayerMode);
    }
}

void MainWindow::on_actionEdit_bounds_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        pview->setEditMode(TileView::eBoundMode);
    }
}

void MainWindow::on_actionAdd_world_triggered()
{
    TileWorld* pworld = NewWorldDialog::getWorld();
    if ( pworld != NULL )
    {
        QString filename = QFileDialog::getSaveFileName(NULL, tr("Save Project"), mpProject->getFolder(), tr("Crafter World Files (*.jwl)"));
        if ( !filename.isEmpty() )
        {
            pworld->setFileName(filename);

            mpProject->addWorld(pworld);
        }
        else
        {
            delete pworld;
        }
    }
}

void MainWindow::on_actionAdd_layer_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        LayerDefinition* pdefinition = NewLayerDialog::getLayer();
        if ( pdefinition != NULL )
        {
            pview->getWorld().addMap(pdefinition);
        }
    }
}

void MainWindow::on_actionViewProject_triggered()
{
    showPanel(mpProjectPanel, ui->actionViewProject->isChecked());
}

void MainWindow::on_actionViewLayers_triggered()
{
    showPanel(mpLayerPanel, ui->actionViewLayers->isChecked());
}

void MainWindow::on_actionViewTiles_triggered()
{
    showPanel(mpTilesPanel, ui->actionViewTiles->isChecked());
}

void MainWindow::on_actionProject_Fix_world_triggered()
{
    if ( mpProject != NULL )
    {
        mpProject->fixMaps();

        repaint();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog;
    dialog.exec();
}
