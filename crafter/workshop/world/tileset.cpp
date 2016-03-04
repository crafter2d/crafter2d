#include "tileset.h"

#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include <qmath.h>

#include "../project.h"

TileSet::TileSet():
    Resource(Resource::eTileSet),
    mTiles(),
    mTileSize()
{
}

TileSet::TileSet(const TileSet& that):
    Resource(that),
    mTiles(that.mTiles),
    mTileSize(that.mTileSize)
{
}

TileSet& TileSet::operator=(TileSet&& that)
{
    Resource::operator =(that);
    mTiles = std::move(that.mTiles);
    mTileSize = that.mTileSize;
    return *this;
}

// - Get/set

void TileSet::update()
{
    if ( !mTiles.empty() )
    {
        auto& atlas = Project::getActiveProject().getSpriteAtlas();
        mTileSize = atlas.bounds(mTiles[0].spriteindex).size();

        emit updated();
    }
}

// - Query

bool TileSet::empty() const
{
    return mTiles.empty();
}

bool TileSet::contains(const QString& name) const
{
    for ( const Tile& tile : mTiles )
    {
        if ( tile.name == name )
        {
            return true;
        }
    }
    return false;
}

QString TileSet::getPath() const
{
    QString path;
    if ( !mTiles.empty() )
    {
        const Tile& tile = mTiles[0];
        int index = tile.name.lastIndexOf('/');
        if ( index > 0 )
        {
            path = tile.name.left(index);
        }
    }
    return path;
}

QString TileSet::getFullPath() const
{
    return Project::getActiveProject().getImagePath() + '/' + getPath();
}

// - Operations

void TileSet::add(const QString& name, int spriteindex)
{
    Tile tile;
    tile.name = name;
    tile.spriteindex = spriteindex;
    mTiles.append(tile);
}

void TileSet::clear()
{
    mTiles.clear();
    mTileSize = QSize(0, 0);
}

void TileSet::paintTile(QPainter& painter, const QPoint& pos, int index) const
{
    if ( index < mTiles.size() )
    {
        const Tile& tile = mTiles[index];
        Project::getActiveProject().getSpriteAtlas().paint(painter, pos + tile.offset, tile.spriteindex);
    }
}
