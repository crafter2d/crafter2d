#include "tile.h"

#include "world/tileset.h"

Tile::Tile():
    QObject(nullptr),
    mTexCoord(-1, -1)
{
}

Tile::Tile(const Tile& that):
    QObject(nullptr),
    mTexCoord(that.mTexCoord)
{
}

Tile::Tile(QPoint &texcoord):
    QObject(nullptr),
    mTexCoord(texcoord)
{
}

Tile::Tile(int x, int y):
    QObject(nullptr),
    mTexCoord(x, y)
{
}

bool Tile::operator==(const Tile& that) const
{
    return mTexCoord == that.mTexCoord;
}

bool Tile::operator!=(const Tile& that) const
{
    return mTexCoord != that.mTexCoord;
}

Tile& Tile::operator=(const Tile& that)
{
    mTexCoord = that.mTexCoord;
    return *this;
}

Tile& Tile::operator=(Tile&& that)
{
    mTexCoord = that.mTexCoord;
    return *this;
}

// - Get/set

const QPoint& Tile::getTexCoord() const
{
    return mTexCoord;
}

bool Tile::isValid() const
{
    return mTexCoord.x() >= 0 && mTexCoord.y() >= 0;
}

