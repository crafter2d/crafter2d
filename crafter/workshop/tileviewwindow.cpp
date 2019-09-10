#include "tileviewwindow.h"

#include <QScrollArea>

#include "world/tileworld.h"

#include "tileview.h"

TileViewWindow::TileViewWindow(QWidget *parent) :
    QScrollArea(parent),
    mpTileView(nullptr)
{
    mpTileView = new TileView();

    setBackgroundRole(QPalette::Window);
    setWidget(mpTileView);
}

// - Get/set

TileView& TileViewWindow::getTileView()
{
    Q_ASSERT(mpTileView != NULL);
    return *mpTileView;
}

bool TileViewWindow::hasWorld() const
{
    return mpTileView->hasWorld();
}

TileWorld& TileViewWindow::getWorld()
{
    return mpTileView->getWorld();
}

void TileViewWindow::setWorld(TileWorld& world)
{
    mpTileView->setWorld(world);

    setWindowTitle(world.getName());
}

void TileViewWindow::setActiveTile(int tile)
{
    mpTileView->setActiveTile(tile);
}
