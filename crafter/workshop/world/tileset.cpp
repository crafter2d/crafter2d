#include "tileset.h"

#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include <qmath.h>

#include "../project.h"

#include "tile.h"

TileSet::TileSet():
    Resource(Resource::eTileSet),
    mTileMap(),
    mTexture(),
    mTiles(),
    mTileSize(),
    mTileCount(-1)
{
}

const Tile& TileSet::operator[](int index) const
{
    return mTiles[index];
}

// - Get/set

bool TileSet::hasTexture() const
{
    return !mTexture.isNull();
}

const QImage& TileSet::getTexture() const
{
    return mTexture;
}

void TileSet::setTexture(const QImage &texture)
{
    mTexture = texture;
}

const QString& TileSet::getTileMap() const
{
    return mTileMap;
}

void TileSet::setTileMap(const QString &map, bool forceReload)
{
    if ( mTileMap != map || forceReload )
    {
        mTileMap = map;

        loadTexture();
    }
}

const QSize& TileSet::getTileSize() const
{
    return mTileSize;
}

void TileSet::setTileSize(const QSize& size)
{
    mTileSize = size;
}

int TileSet::getTileCount() const
{
    return mTileCount;
}

void TileSet::setTileCount(int count)
{
    mTileCount = count;
}

// - Query

int TileSet::indexOf(const Tile& item) const
{
    return mTiles.indexOf(item);
}

// - Operations

void TileSet::loadTexture()
{
    QPixmap pixmap;
    if ( pixmap.load(Project::getActiveProject().getFilePath(mTileMap)) )
    {
        mTexture = pixmap.toImage();

        generateTiles();
    }
}

void TileSet::generateTiles()
{
    Q_ASSERT(!mTexture.isNull());

    mTiles.clear();

    int maxFramesPerRow = static_cast<int>(mTexture.width() / mTileSize.width());

    // build the texture coord lookup table
    for (int tc = 0; tc < mTileCount; tc++)
    {
        // calculate starting texture coordinates
        int x = (tc % maxFramesPerRow) * mTileSize.width();
        int y = (int) qFloor((float)tc / maxFramesPerRow) * mTileSize.height();

        mTiles.append(Tile(x, y));
    }
}

void TileSet::paintTile(QPainter& painter, int index, int x, int y) const
{
    Q_ASSERT(!mTexture.isNull());
    if ( index < mTiles.size() )
    {
        const Tile& tile = mTiles[index];
        const QPoint& texcoords = tile.getTexCoord();
        painter.drawImage(x, y,
                          mTexture,
                          texcoords.x(),
                          texcoords.y(),
                          mTileSize.width(),
                          mTileSize.height());
    }
}
