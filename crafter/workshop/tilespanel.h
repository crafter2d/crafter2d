#ifndef TILESPANEL_H
#define TILESPANEL_H

#include "dockpanel.h"

namespace Ui {
class TilesPanel;
}

class QItemSelection;

class TileMap;
class TileModel;
class TileSelector;

class TilesPanel : public DockPanel
{
    Q_OBJECT

public:
    explicit TilesPanel(MainWindow& mainwindow);
    ~TilesPanel();

protected:

  // notifications
    virtual void worldActivated(TileWorld* pworld) override;

private slots:
    void on_tile_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void on_world_activeMapChanged(TileMap *map);

private:
    Ui::TilesPanel *ui;
    TileModel* mpTileModel;
    TileSelector* mpSelector;
};

#endif // TILESPANEL_H
