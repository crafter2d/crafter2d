#include "tileset.h"

#include <QDir>
#include <QImage>
#include <QPixmap>

#include "../project.h"

QTileSet::QTileSet():
    mTexture(),
    mTileMap(),
    mTileSize(),
    mTileCount(-1)
{
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

// - Operations

void QTileSet::loadTexture()
{
    QPixmap pixmap;
    if ( pixmap.load(Project::getActiveProject().getFilePath(mTileMap)) )
    {
        mTexture = pixmap.toImage();
    }
}
