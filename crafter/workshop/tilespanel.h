#ifndef TILESPANEL_H
#define TILESPANEL_H

#include "dockpanel.h"

namespace Ui {
class TilesPanel;
}

class Tile;
class TileMap;
class TileSelector;

class TilesPanel : public DockPanel
{
    Q_OBJECT

public:
    explicit TilesPanel(MainWindow& mainwindow);
    virtual ~TilesPanel();

protected:

  // notifications
    virtual void worldActivated(TileWorld* pworld) override;

private slots:
    void on_tileSelected(const Tile *tile);
    void on_world_activeMapChanged(TileMap *map);

private:
    Ui::TilesPanel *ui;
    TileSelector* mpSelector;
};

#endif // TILESPANEL_H
