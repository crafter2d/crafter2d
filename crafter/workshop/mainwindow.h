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
    TileViewWindow* findWindow(TileWorld* pworld);

signals:
    void projectChanged(Project* newProject);

protected:
 // event handlers
    virtual void closeEvent(QCloseEvent *pevent) override;

private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionOpenRecentFile_triggered();

    void on_actionEdit_layers_triggered();
    void on_actionEdit_bounds_triggered();

    void on_actionViewProject_triggered();
    void on_actionViewLayers_triggered();
    void on_actionViewTiles_triggered();

    void on_actionAdd_world_triggered();
    void on_actionAdd_layer_triggered();

    void on_actionProject_Fix_world_triggered();

    void on_actionE_xit_triggered();

    void on_actionAbout_triggered();

    void on_actionEdit_Undo_triggered();

    void on_actionEdit_Redo_triggered();

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
