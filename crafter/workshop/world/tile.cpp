#include "tile.h"

#include <QPainter>

#include "world/tileset.h"

Tile::Tile():
    QObject(nullptr),
    mpTileSet(NULL),
    mTexCoord()
{
}

Tile::Tile(const Tile& that):
    QObject(nullptr),
    mpTileSet(that.mpTileSet),
    mTexCoord(that.mTexCoord)
{
}

Tile::Tile(QTileSet &tileset, QPoint &texcoord):
    QObject(nullptr),
    mpTileSet(&tileset),
    mTexCoord(texcoord)
{
}

Tile& Tile::operator=(const Tile& that)
{
    mpTileSet = that.mpTileSet;
    mTexCoord = that.mTexCoord;
    return *this;
}

// - Get/set

const QSize &Tile::getSize() const
{
    Q_ASSERT(mpTileSet != nullptr);
    return mpTileSet->getTileSize();
}

const QPoint& Tile::getTexCoord() const
{
    return mTexCoord;
}

// - Query

bool Tile::isValid() const
{
    return mpTileSet != nullptr;
}

// - Painting

void Tile::paint(QPainter& painter, int x, int y) const
{
    if ( mpTileSet != nullptr )
    {
        const QImage& texture = mpTileSet->getTexture();
        const QSize& tilesize = mpTileSet->getTileSize();

        painter.drawImage(x, y,
                          texture,
                          mTexCoord.x(),
                          mTexCoord.y(),
                          tilesize.width(),
                          tilesize.height());
    }
}
