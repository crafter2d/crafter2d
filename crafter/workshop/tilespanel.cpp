#include "tilespanel.h"
#include "ui_tilespanel.h"

#include "world/tilemap.h"
#include "world/tileworld.h"

#include "mainwindow.h"
#include "tileview.h"
#include "tileselector.h"

TilesPanel::TilesPanel(MainWindow &mainwindow) :
    DockPanel(mainwindow),
    ui(new Ui::TilesPanel),
    mpSelector(nullptr)
{
    ui->setupUi(this);

    mpSelector = new TileSelector(ui->scrollArea);
    connect(mpSelector, SIGNAL(tileSelected(int)), SLOT(on_tileSelected(int)));
    mpSelector->show();

    ui->scrollArea->setWidget(mpSelector);
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

void TilesPanel::on_tileSelected(int tile)
{
    TileView* pview = getMainWindow().getActiveView();
    if ( pview != NULL )
    {
        pview->setActiveTile(tile);
    }
}

void TilesPanel::on_world_activeMapChanged(TileMap* tilemap)
{
    const TileSet* pset = nullptr;
    if ( tilemap != nullptr )
    {
        pset = &tilemap->getTileSet();
    }

    mpSelector->setTileSet(pset);
}
