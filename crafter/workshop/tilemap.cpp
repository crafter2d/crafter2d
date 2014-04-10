#include "tilemap.h"

#include <QPainter>
#include <QString>
#include <QFile>
#include <QDir>

#include "world/tileset.h"
#include "world/tilesetreader.h"

#include "project.h"
#include "tile.h"

TileMap::TileMap(const TileMapDesc& desc):
    mDesc(desc),
    mpField(NULL),
    mpTileSet(NULL),
    mTiles(),
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

const QSize& TileMap::getTileSize() const
{
    return mpTileSet->getTileSize();
}

int TileMap::getTileCount() const
{
    return mTiles.size();
}

Tile& TileMap::getTile(int tile)
{
    return *mTiles[tile];
}

int TileMap::indexOf(const Tile& tile) const
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

// - Painting

void TileMap::paint(QPainter& painter)
{
    if ( mpTileSet != NULL && mpTileSet->hasTexture() )
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
                        Tile* ptile = mTiles[tile];
                        paint(painter, *ptile, posx, posy);
                    }
                }

                posx += tilesize.width();
            }

            posy += tilesize.height();
        }
    }
}

void TileMap::paint(QPainter& painter, Tile& tile, int x, int y)
{
    Q_ASSERT(mpTileSet != NULL);
    const QPoint& texcoords = tile.getTexCoord();
    painter.drawImage(x, y, mpTileSet->getTexture(),
                      texcoords.x(),
                      texcoords.y(),
                      mpTileSet->getTileSize().width(),
                      mpTileSet->getTileSize().height());
}

// - Operations

void TileMap::setField(TileField* pfield)
{
    mpField = pfield;
}

void TileMap::setTileSet(QTileSet* ptileset)
{
    if ( mpTileSet != ptileset )
    {
        mpTileSet = ptileset;

        generateTiles();

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

void TileMap::generateTiles()
{
    Q_ASSERT(mpTileSet != NULL);
    if ( mpTileSet->hasTexture() )
    {
        const QImage& image = mpTileSet->getTexture();
        const QSize& tilesize = mpTileSet->getTileSize();
        int maxFramesPerRow = static_cast<int>(image.width() / tilesize.width());

        // build the texture coord lookup table
        for (int tc = 0; tc < mpTileSet->getTileCount(); tc++)
        {
            // calculate starting texture coordinates
            int x = (tc % maxFramesPerRow) * tilesize.width();
            int y = (int) floorf ((float)tc / maxFramesPerRow) * tilesize.height();

            mTiles.append(new Tile(*this, QPoint(x, y)));
        }
    }
}

Tile TileMap::getTile(const QPoint& mousepos, TileField::Level level) const
{
    const QSize& tilesize = mpTileSet->getTileSize();

    int tilex = mousepos.x() / tilesize.width();
    int tiley = mousepos.y() / tilesize.height();

    Tile result;
    if ( tilex >= 0 && tilex < mDesc.size.width()
      && tiley >= 0 && tiley < mDesc.size.height() )
    {
        int index = mpField->get(level, tilex, tiley);
        if ( index < mTiles.size() )
        {
            result = *mTiles[index];
        }
    }

    return result;
}

bool TileMap::setTile(const QPoint &mousepos, TileField::Level level, int tileindex)
{
    const QSize& tilesize = mpTileSet->getTileSize();

    int tilex = mousepos.x() / tilesize.width();
    int tiley = mousepos.y() / tilesize.height();

    if ( tilex >= 0 && tilex < mDesc.size.width()
      && tiley >= 0 && tiley < mDesc.size.height() )
    {
        mpField->set(level, tilex, tiley, tileindex);
        return true;
    }

    return false;
}

/// Set the map cell at position mouspos to tile. If tile is invalid the cell is cleared.
bool TileMap::setTile(const QPoint& mousepos, TileField::Level level, const Tile& tile)
{
    int index = 255;
    if ( tile.isValid() )
    {
        index = indexOf(tile);
    }

    return setTile(mousepos, level, index);
}

void TileMap::clearTile(const QPoint& mousepos, TileField::Level level)
{
    setTile(mousepos, level, 255);
}
