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
class Entity;
class Project;
class TileMap;
class TileView;
class LayerPanel;
class OutputPanel;
class ProjectPanel;
class ScriptFile;
class ScriptView;
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
    void showEntity(Entity& entity);
    void showWorld(TileWorld& world);
    ScriptView *showScript(ScriptFile& script);

    Entity* getSelectedEntity() { return mpSelectedEntity; }
    void setSelectedEntity(Entity* pentity);

  // error handling
    void gotoError(const QString& classname, int line);

 // searching
    TileViewWindow* findWindow(TileWorld &world);
    ScriptView*     findWindow(ScriptFile& file);

signals:
    void projectChanged(Project* newProject);

protected:
 // event handlers
    virtual void closeEvent(QCloseEvent *pevent) override;

private slots:
    void on_actionFile_NewProject_triggered();
    void on_actionFile_NewWorld_triggered();
    void on_actionFile_NewLayer_triggered();
    void on_actionFile_NewScript_triggered();
    void on_actionFile_Save_triggered();
    void on_actionFile_Open_triggered();
    void on_actionFile_OpenRecentFile_triggered();
    void on_actionFile_Exit_triggered();

    void on_actionEdit_Undo_triggered();
    void on_actionEdit_Redo_triggered();
    void on_actionEdit_Layers_triggered();
    void on_actionEdit_Bounds_triggered();
    void on_actionEdit_Objects_triggered();

    void on_actionProject_Build_triggered();
    void on_actionProject_Run_triggered();

    void on_actionView_Project_triggered();
    void on_actionView_Layers_triggered();
    void on_actionView_Tiles_triggered();

    void on_actionAbout_triggered();

    void on_actionView_Output_triggered();

    void on_actionTools_TextureSplitter_triggered();

    void on_actionTools_TexturePacker_triggered();

private:
    enum { MaxRecentFiles = 5 };

    template<class V, class E>
    V* findWindow(E& element);

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
    Entity*  mpSelectedEntity;

    ProjectPanel*   mpProjectPanel;
    LayerPanel*     mpLayerPanel;
    TilesPanel*     mpTilesPanel;
    OutputPanel*    mpOutputPanel;
};

#endif // MAINWINDOW_H
