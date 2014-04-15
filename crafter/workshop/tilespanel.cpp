#include "tilespanel.h"
#include "ui_tilespanel.h"

#include "mainwindow.h"
#include "tilemap.h"
#include "tileview.h"
#include "tileworld.h"
#include "tileselector.h"

TilesPanel::TilesPanel(MainWindow &mainwindow) :
    DockPanel(mainwindow),
    ui(new Ui::TilesPanel),
    mpSelector(nullptr)
{
    ui->setupUi(this);

    mpSelector = new TileSelector(0);
    connect(mpSelector, SIGNAL(tileSelected(const Tile*)), SLOT(on_tileSelected(const Tile*)));

    ui->verticalLayout->addWidget(mpSelector);
}

TilesPanel::~TilesPanel()
{
    delete ui;
}

// - Notifications

void TilesPanel::worldActivated(TileWorld* pworld)
{
    TileMap* pmap = nullptr;
    if ( pworld != NULL )
    {
        connect(pworld, SIGNAL(activeMapChanged(TileMap*)), SLOT(on_world_activeMapChanged(TileMap*)));

        if ( pworld->hasActiveMap() )
        {
            pmap = pworld->getActiveMap();
        }
    }

    on_world_activeMapChanged(pmap);
}

// - Slots

void TilesPanel::on_tileSelected(const Tile* tile)
{
    TileView* pview = getMainWindow().getActiveView();
    if ( pview != NULL )
    {
        pview->setActiveTile(tile);
    }
}

void TilesPanel::on_world_activeMapChanged(TileMap* tilemap)
{
    const QTileSet* pset = nullptr;
    if ( tilemap != nullptr )
    {
        pset = &tilemap->getTileSet();
    }

    mpSelector->setTileSet(pset);
}
