#include "tileselector.h"

#include <QPainter>

#include <qmath.h>

#include "world/tileset.h"
#include "tile.h"

TileSelector::TileSelector(QWidget *parent) :
    QWidget(parent),
    mpTileSet(nullptr),
    mHorizontalTiles(0)
{
}

// - Get/set
const QTileSet& TileSelector::getTileSet() const
{
    Q_ASSERT(mpTileSet != nullptr);
    return *mpTileSet;
}

void TileSelector::setTileSet(const QTileSet *ptileset)
{
    if ( mpTileSet != ptileset )
    {
        mpTileSet = ptileset;
        tilesetChanged();
    }
}

// - Overrides

QSize TileSelector::sizeHint() const
{
    QSize result;

    if ( mpTileSet != nullptr )
    {
        const QSize& size = mpTileSet->getTileSize();

        int width = size.width() * 5 + 16; // = 4x4 border
        int lines = qCeil(mpTileSet->getTileCount() / 5.0f);
        int height = lines * size.height() + (lines-1) * 4;

        result.setWidth(width);
        result.setHeight(height);
    }

    return result;
}

void TileSelector::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if ( mpTileSet != nullptr )
    {
        int count = mpTileSet->getTileCount();

        int x = 2, y = 2;
        for ( int index = 0; index < count; ++index )
        {
            const Tile& tile = (*mpTileSet)[index];

            tile.paint(painter, x, y);

            if ( index % mHorizontalTiles == 0 )
            {
                x = 2;
                y += mpTileSet->getTileSize().height() + 4;
            }
            else
            {
                x += mpTileSet->getTileSize().width() + 4;
            }
        }
    }
}

void TileSelector::resizeEvent(QResizeEvent *e)
{
    tilesetChanged();
}

// - Operations

void TileSelector::tilesetChanged()
{
    if ( mpTileSet != nullptr )
    {
        const QSize& size = mpTileSet->getTileSize();
        const QSize& windowSize = this->size();

        mHorizontalTiles = qFloor(windowSize.width() / (size.width() + 2));

        const QSize& ps = ((QWidget*)parent())->size();
        setMinimumSize(ps);
    }
}

