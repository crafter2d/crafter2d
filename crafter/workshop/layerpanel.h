#ifndef LAYERPANEL_H
#define LAYERPANEL_H

#include "dockpanel.h"

namespace Ui {
class LayerPanel;
}

#include "world/tilefield.h"

class QItemSelection;
class QMdiSubWindow;

class MainWindow;

class LayerPanel : public DockPanel
{
    Q_OBJECT

public:
    explicit LayerPanel(MainWindow& parent);
    ~LayerPanel();

  // get/set
    void setWorld(TileWorld& world);

signals:
    void worldChanged();

private slots:
    void on_layerSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    void on_buttonMoveUp_clicked();
    void on_buttonMoveDown_clicked();

    void on_deleteact_triggered();
    void on_resizeact_triggered();

    void on_buttonFront_clicked();
    void on_buttonMid_clicked();
    void on_buttonBack_clicked();

protected:
  // overrides
    virtual void worldActivated(TileWorld *pworld) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

private:

  // operations
    void createContextMenu();
    void setActiveWorld();
    void setLevel(TileField::Level level);

    Ui::LayerPanel *ui;

    QAction* mpDeleteAct;
    QAction* mpResizeAct;
};

#endif // LAYERPANEL_H
