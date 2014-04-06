#include "tile.h"

#include "tilemap.h"

Tile::Tile():
    QObject(nullptr),
    mpMap(NULL),
    mTexCoord()
{
}

Tile::Tile(const Tile& that):
    QObject(nullptr),
    mpMap(that.mpMap),
    mTexCoord(that.mTexCoord)
{
}

Tile::Tile(TileMap &map, QPoint &texcoord):
    QObject(nullptr),
    mpMap(&map),
    mTexCoord(texcoord)
{
}

Tile& Tile::operator=(const Tile& that)
{
    mpMap = that.mpMap;
    mTexCoord = that.mTexCoord;
    return *this;
}

// - Get/set

QSize Tile::getSize() const
{
    return mpMap != nullptr ? mpMap->getTileSize() : QSize();
}

const QPoint& Tile::getTexCoord() const
{
    return mTexCoord;
}

// - Query

bool Tile::isValid() const
{
    return mpMap != nullptr;
}

// - Painting

void Tile::paint(QPainter& painter, int x, int y)
{
    if ( mpMap != nullptr )
    {
        mpMap->paint(painter, *this, x, y);
    }
}
