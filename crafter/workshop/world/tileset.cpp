#include "tileset.h"

#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include <qmath.h>

#include "../project.h"

#include "tile.h"

QTileSet::QTileSet():
    Resource(Resource::eTileSet),
    mTileMap(),
    mTexture(),
    mTiles(),
    mTileSize(),
    mTileCount(-1)
{
}

const Tile& QTileSet::operator[](int index) const
{
    return *mTiles[index];
}

// - Get/set

bool QTileSet::hasTexture() const
{
    return !mTexture.isNull();
}

const QImage& QTileSet::getTexture() const
{
    return mTexture;
}

void QTileSet::setTexture(const QImage &texture)
{
    mTexture = texture;
}

const QString& QTileSet::getTileMap() const
{
    return mTileMap;
}

void QTileSet::setTileMap(const QString &map, bool forceReload)
{
    if ( mTileMap != map || forceReload )
    {
        mTileMap = map;

        loadTexture();
    }
}

const QSize& QTileSet::getTileSize() const
{
    return mTileSize;
}

void QTileSet::setTileSize(const QSize& size)
{
    mTileSize = size;
}

int QTileSet::getTileCount() const
{
    return mTileCount;
}

void QTileSet::setTileCount(int count)
{
    mTileCount = count;
}

// - Query

int QTileSet::indexOf(const Tile& tile) const
{
    for ( int index = 0; index < mTiles.size(); index++ )
    {
        Tile* ptile = mTiles[index];
        if ( ptile->getTexCoord() == tile.getTexCoord() )
        {
            return index;
        }
    }
    return -1;
}

// - Operations

void QTileSet::loadTexture()
{
    QPixmap pixmap;
    if ( pixmap.load(Project::getActiveProject().getFilePath(mTileMap)) )
    {
        mTexture = pixmap.toImage();

        generateTiles();
    }
}

void QTileSet::generateTiles()
{
    Q_ASSERT(!mTexture.isNull());

    int maxFramesPerRow = static_cast<int>(mTexture.width() / mTileSize.width());

    // build the texture coord lookup table
    for (int tc = 0; tc < mTileCount; tc++)
    {
        // calculate starting texture coordinates
        int x = (tc % maxFramesPerRow) * mTileSize.width();
        int y = (int) qFloor((float)tc / maxFramesPerRow) * mTileSize.height();

        mTiles.append(new Tile(*this, QPoint(x, y)));
    }
}

void QTileSet::paintTile(QPainter& painter, int index, int x, int y)
{
        Q_ASSERT(!mTexture.isNull());
        const Tile* ptile = mTiles[index];
        const QPoint& texcoords = ptile->getTexCoord();
        painter.drawImage(x, y,
                          mTexture,
                          texcoords.x(),
                          texcoords.y(),
                          mTileSize.width(),
                          mTileSize.height());
}
