#ifndef DOCKPANEL_H
#define DOCKPANEL_H

#include <QDockWidget>
#include <QMdiSubWindow>

class MainWindow;
class TileWorld;

class DockPanel : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockPanel(MainWindow& parent);

signals:

protected:
  // get/set
    MainWindow& getMainWindow();

  // notifications
    virtual void worldActivated(TileWorld* pworld) = 0;

private slots:
    void on_subWindowActivated(QMdiSubWindow *psubwindow);
};

#endif // DOCKPANEL_H
