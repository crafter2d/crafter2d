#include "tilemap.h"

#include <QPainter>
#include <QString>
#include <QFile>
#include <QDir>

#include "world/tileset.h"
#include "world/tilesetreader.h"

#include "project.h"

TileMap::TileMap(const TileMapDesc& desc):
    mDesc(desc),
    mpField(NULL),
    mpTileSet(NULL),
    mPixelSize()
{
}

// - Query

const QString& TileMap::getName() const
{
    return mDesc.name;
}

TileMapDesc& TileMap::getDesc()
{
    return mDesc;
}

const TileMapDesc& TileMap::getDesc() const
{
    return mDesc;
}

void TileMap::setDesc(const TileMapDesc& desc)
{
    mDesc = desc;
}

const TileField &TileMap::getField() const
{
    return *mpField;
}

TileField &TileMap::getField()
{
    return *mpField;
}

const QSize& TileMap::getMinimumSize() const
{
    return mPixelSize;
}

const TileSet& TileMap::getTileSet() const
{
    Q_ASSERT(mpTileSet != nullptr);
    return *mpTileSet;
}

// - Painting

void TileMap::paint(QPainter& painter)
{
    if ( mpTileSet != NULL )
    {
        int posy = 0;
        const QSize& tilesize = mpTileSet->getTileSize();
        for ( int y = 0; y < mDesc.size.height(); ++y )
        {
            int posx = 0;
            for ( int x = 0; x < mDesc.size.width(); ++x )
            {
                for ( int level = 2; level >= 0; --level )
                {
                    int tile = mpField->get((TileField::Level)level, x, y);
                    if ( tile < 255 )
                    {
                        mpTileSet->paintTile(painter, tile, posx, posy);
                    }
                }

                posx += tilesize.width();
            }

            posy += tilesize.height();
        }
    }
}

// - Operations

void TileMap::setField(TileField* pfield)
{
    mpField = pfield;
}

void TileMap::setTileSet(TileSet* ptileset)
{
    if ( mpTileSet != ptileset )
    {
        mpTileSet = ptileset;

        const QSize& tilesize = mpTileSet->getTileSize();
        mPixelSize.setWidth(tilesize.width() * mDesc.size.width());
        mPixelSize.setHeight(tilesize.height() * mDesc.size.height());
    }
}

void TileMap::resize(const QSize& size)
{
    QSize currentsize(mDesc.size);
    if ( size != currentsize )
    {
        mpField->resize(size);

        mDesc.size = size;

        const QSize& tilesize = mpTileSet->getTileSize();
        mPixelSize.setWidth(tilesize.width() * mDesc.size.width());
        mPixelSize.setHeight(tilesize.height() * mDesc.size.height());

        emit mapChanged(*this);
    }
}

int TileMap::getTile(const QPoint& mousepos, TileField::Level level) const
{
    const QSize& tilesize = mpTileSet->getTileSize();

    int tilex = mousepos.x() / tilesize.width();
    int tiley = mousepos.y() / tilesize.height();

    if ( tilex >= 0 && tilex < mDesc.size.width()
      && tiley >= 0 && tiley < mDesc.size.height() )
    {
        return mpField->get(level, tilex, tiley);
    }

    return TileSet::INVALID_TILE;
}

/// Set the map cell at position mouspos to tile. If tile is invalid the cell is cleared.
bool TileMap::setTile(const QPoint &mousepos, TileField::Level level, int tile)
{
    const QSize& tilesize = mpTileSet->getTileSize();

    int tilex = mousepos.x() / tilesize.width();
    int tiley = mousepos.y() / tilesize.height();

    if ( tilex >= 0 && tilex < mDesc.size.width()
      && tiley >= 0 && tiley < mDesc.size.height() )
    {
        mpField->set(level, tilex, tiley, tile);
        return true;
    }

    return false;
}

void TileMap::clearTile(const QPoint& mousepos, TileField::Level level)
{
    setTile(mousepos, level, TileSet::INVALID_TILE);
}
