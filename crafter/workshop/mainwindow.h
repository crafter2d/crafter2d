#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QItemSelection;
class QMdiSubWindow;
class QMdiArea;

class DockPanel;
class Project;
class TileMap;
class TileView;
class LayerPanel;
class ProjectPanel;
class TilesPanel;
class TileViewWindow;
class TileWorld;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

 // query
    QMdiArea& getMdiArea();
    TileView *getActiveView();

 // operations
    void addWindow(TileWorld& world);

 // searching
    TileViewWindow* findWindow(TileWorld &world);

signals:
    void projectChanged(Project* newProject);

protected:
 // event handlers
    virtual void closeEvent(QCloseEvent *pevent) override;

private slots:
    void on_actionFile_NewProject_triggered();
    void on_actionFile_NewWorld_triggered();
    void on_actionFile_NewLayer_triggered();
    void on_actionFile_Save_triggered();
    void on_actionFile_Open_triggered();
    void on_actionFile_OpenRecentFile_triggered();
    void on_actionFile_Exit_triggered();

    void on_actionEdit_Layers_triggered();
    void on_actionEdit_Bounds_triggered();

    void on_actionView_Project_triggered();
    void on_actionView_Layers_triggered();
    void on_actionView_Tiles_triggered();

    void on_actionAbout_triggered();

    void on_actionFile_ImportWorld_triggered();

    void on_actionFile_NewScript_triggered();

private:
    enum { MaxRecentFiles = 5 };

 // operations
    void setProject(Project* pproject);
    void setCurrentFile(const QString& fileName);

    void loadProject(const QString& fileName);
    void saveProject();

    void setRecentFileActions();
    void updateRecentFileList();

 // settings
    void readSettings();
    void writeSettings();

 // toolpanes
    void restorePanels();
    void showPanel(DockPanel* ppanel, bool show);
    void showLayersPanel(bool show);
    void showTilesPanel(bool show);

    QString strippedName(const QString& fullFileName);

 // data
    Ui::MainWindow *ui;
    Project* mpProject;
    QAction* mpRecentFileActions[MaxRecentFiles];

    ProjectPanel*   mpProjectPanel;
    LayerPanel*     mpLayerPanel;
    TilesPanel*     mpTilesPanel;
};

#endif // MAINWINDOW_H
