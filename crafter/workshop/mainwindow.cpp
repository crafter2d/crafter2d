#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QProgressBar>

#include "texture/texturesplitter.h"
#include "world/tileworld.h"

#include "aboutdialog.h"
#include "newprojectdialog.h"
#include "newlayerdialog.h"
#include "newscriptdialog.h"
#include "outputpanel.h"
#include "project.h"
#include "projectpanel.h"
#include "layerpanel.h"
#include "tilespanel.h"
#include "tileview.h"
#include "tileviewwindow.h"
#include "worldwizard.h"
#include "scriptview.h"

#pragma warning (disable: 4351)

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
    pgroup->addAction(ui->actionEdit_Layers);
    pgroup->addAction(ui->actionEdit_Bounds);
    pgroup->addAction(ui->actionEdit_Objects);

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

        connect(paction, SIGNAL(triggered()), this, SLOT(on_actionFile_OpenRecentFile_triggered()));
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

    mpOutputPanel = new OutputPanel(*this);
    addDockWidget(Qt::BottomDockWidgetArea, mpOutputPanel);
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

void MainWindow::showWorld(TileWorld& world)
{
    TileViewWindow* pwindow = findWindow(world);
    if ( pwindow != NULL )
    {
        ui->centralWidget->setActiveSubWindow(dynamic_cast<QMdiSubWindow*>(pwindow->parent()));
        pwindow->activateWindow();
    }
    else
    {
        TileViewWindow* pwindow = new TileViewWindow();
        pwindow->setWorld(world);
        pwindow->show();

        ui->centralWidget->addSubWindow(pwindow);
    }
}

ScriptView* MainWindow::showScript(ScriptFile& script)
{
    ScriptView* pview = findWindow(script);
    if ( pview != NULL )
    {
        ui->centralWidget->setActiveSubWindow(dynamic_cast<QMdiSubWindow*>(pview->parent()));
        pview->activateWindow();
    }
    else
    {
        pview = new ScriptView(script);
        pview->show();

        ui->centralWidget->addSubWindow(pview);
    }

    return pview;
}

// - Error handling

void MainWindow::gotoError(const QString& classname, int line)
{
    ScriptFile* pfile = mpProject->findScript(classname);
    if ( pfile != nullptr )
    {
        ScriptView* pview = showScript(*pfile);
        pview->setCurrentLine(line);
    }
}

// - Search

TileViewWindow* MainWindow::findWindow(TileWorld& world)
{
    QMdiSubWindow* psubwindow = NULL;
    QList<QMdiSubWindow*> subwindows = ui->centralWidget->subWindowList();
    foreach (psubwindow, subwindows)
    {
        TileViewWindow* pwindow = static_cast<TileViewWindow*>(psubwindow->widget());
        TileWorld& viewworld = pwindow->getTileView().getWorld();
        if ( &viewworld == &world )
        {
            return pwindow;
        }
    }
    return NULL;
}

ScriptView* MainWindow::findWindow(ScriptFile& file)
{
    QMdiSubWindow* psubwindow = NULL;
    QList<QMdiSubWindow*> subwindows = ui->centralWidget->subWindowList();
    foreach (psubwindow, subwindows)
    {
        ScriptView* pview = dynamic_cast<ScriptView*>(psubwindow->widget());
        if ( pview != nullptr && pview->hasScriptFile() && &pview->getScriptFile() == &file )
        {
            return pview;
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

void MainWindow::on_actionFile_NewProject_triggered()
{
    Project* pproject = Project::createNew(this);
    if ( pproject != NULL )
    {
        setProject(pproject);
        saveProject();
    }
}

void MainWindow::on_actionFile_NewWorld_triggered()
{
    if ( mpProject != NULL )
    {
        TileWorld* pworld = WorldWizard::show();

        if ( pworld != NULL )
        {
            mpProject->addWorld(pworld);
        }
        else
        {
            delete pworld;
        }
    }
}

void MainWindow::on_actionFile_NewLayer_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        TileMap* pmap = NewLayerDialog::getMap();
        if ( pmap != NULL )
        {
            pview->getWorld().addMap(pmap);
        }
    }
}

void MainWindow::on_actionFile_NewScript_triggered()
{
    ScriptFile* pscript = NewScriptDialog::New(this, *mpProject);
    if ( pscript != NULL )
    {
        showScript(*pscript);
    }
}

void MainWindow::on_actionFile_Save_triggered()
{
    saveProject();
}

void MainWindow::on_actionFile_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), "e:/devel", tr("Crafter Project Files (*.craft)"));

    if ( !fileName.isEmpty() )
    {
        loadProject(fileName);
    }
}

void MainWindow::on_actionFile_OpenRecentFile_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if ( action != NULL )
    {
        loadProject(action->data().toString());
    }
}

void MainWindow::on_actionFile_Exit_triggered()
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

void MainWindow::on_actionEdit_Layers_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        pview->setEditMode(TileView::eLayerMode);
    }
}

void MainWindow::on_actionEdit_Bounds_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        pview->setEditMode(TileView::eBoundMode);
    }
}

void MainWindow::on_actionEdit_Objects_triggered()
{
    TileView* pview = getActiveView();
    if ( pview != NULL )
    {
        pview->setEditMode(TileView::eObjectMode);
    }
}

void MainWindow::on_actionProject_Build_triggered()
{
    if ( mpProject != NULL )
    {
        mpProject->build();
    }
}

void MainWindow::on_actionProject_Run_triggered()
{
    if ( mpProject != NULL )
    {
        mpProject->run();
    }
}

void MainWindow::on_actionView_Project_triggered()
{
    showPanel(mpProjectPanel, ui->actionView_Project->isChecked());
}

void MainWindow::on_actionView_Layers_triggered()
{
    showPanel(mpLayerPanel, ui->actionView_Layers->isChecked());
}

void MainWindow::on_actionView_Tiles_triggered()
{
    showPanel(mpTilesPanel, ui->actionView_Tiles->isChecked());
}

void MainWindow::on_actionView_Output_triggered()
{
    showPanel(mpOutputPanel, ui->actionView_Output->isChecked());
}


void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::on_actionTools_TextureSplitter_triggered()
{
    TextureSplitter splitter;
    splitter.exec();
}
