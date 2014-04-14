#include "tilespanel.h"
#include "ui_tilespanel.h"

#include "mainwindow.h"
#include "tiledelegate.h"
#include "tilemap.h"
#include "tilemodel.h"
#include "tileview.h"
#include "tileworld.h"
#include "tileselector.h"

TilesPanel::TilesPanel(MainWindow &mainwindow) :
    DockPanel(mainwindow),
    ui(new Ui::TilesPanel),
    mpTileModel(NULL),
    mpSelector(nullptr)
{
    ui->setupUi(this);

    mpSelector = new TileSelector(0);
    mpSelector->setVisible(true);

    ui->scrollArea->setWidget(mpSelector);

    /*
    mpTileModel = new TileModel();
    ui->listTiles->setModel(mpTileModel);
    ui->listTiles->setItemDelegate(new TileDelegate());

    connect(ui->listTiles->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(on_tile_selectionChanged(QItemSelection,QItemSelection)));*/
}

TilesPanel::~TilesPanel()
{
    delete ui;
}

// - Notifications

void TilesPanel::worldActivated(TileWorld* pworld)
{
    if ( pworld != NULL )
    {
        if ( pworld->hasActiveMap() )
        {
            //mpTileModel->setMap(pworld->getActiveMap());
            const QTileSet& tileset = pworld->getActiveMap()->getTileSet();
            mpSelector->setTileSet(&tileset);
        }

        connect(pworld, SIGNAL(activeMapChanged(TileMap*)), SLOT(on_world_activeMapChanged(TileMap*)));
    }
    else
    {
        //mpTileModel->clearMap();
    }
}

// - Slots

void TilesPanel::on_tile_selectionChanged(const QItemSelection& selected, const QItemSelection& /* deselected */)
{
    /*
    TileView* pview = getMainWindow().getActiveView();
    if ( pview != NULL )
    {
        QModelIndex index;
        foreach(index, selected.indexes())
        {
            if ( index.model() != NULL )
            {
                QVariant var = index.model()->data(index);

                if ( var.canConvert<Tile>() )
                {
                    Tile tile = var.value<Tile>();
                    pview->setActiveTile(tile);
                }
            }
        }
    }
    */
}

void TilesPanel::on_world_activeMapChanged(TileMap* tilemap)
{
    /*
    TileModel* pmodel = dynamic_cast<TileModel*>(ui->listTiles->model());
    pmodel->setMap(tilemap);
    */
}
