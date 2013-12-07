#include "tilemap.h"

#include <QPainter>
#include <QString>

#include <QDir>

#include <engine/world/layer.h>

#include "stringinterface.h"
#include "tile.h"
#include "tileworld.h"

TileMap::TileMap(TileWorld &world, Layer *player):
    mWorld(world),
    mpLayer(player),
    mImage(),
    mSize(),
    mTileSet(),
    mTiles()
{
}

void TileMap::load(const QString& path)
{
    QDir dir(path);
    const LayerDefinition& layerdef = mpLayer->getDefinition();
    QString tilesetpath = dir.absoluteFilePath(QString(layerdef.tileset.toUtf8().c_str()) + ".xml");
    String tileset = toString(tilesetpath);
    mTileSet.load(tileset);

    QString texname = toQString(mTileSet.getTextureName());
    QString mappath = dir.absoluteFilePath(texname);

    QPixmap map;
    if (map.load(mappath))
    {
        mImage = map.toImage();

        generateTiles();
    }
    else
    {
        throw new std::exception("Could not load tile set!");
    }

    int width = mTileSet.getTileWidth();
    int height = mTileSet.getTileHeight();

    mSize.setWidth(width * layerdef.width);
    mSize.setHeight(height * layerdef.height);
}

// - Query

Layer& TileMap::getLayer()
{
    return *mpLayer;
}

const QString TileMap::getName() const
{
    return QString(mpLayer->getName().toUtf8().c_str());
}

const QSize& TileMap::getSize() const
{
    return mSize;
}

QSize TileMap::getTileSize() const
{
    return QSize(mTileSet.getTileWidth(), mTileSet.getTileHeight());
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
    if ( tile.isValid() )
    {
        for ( int index = 0; index < mTiles.size(); index++ )
        {
            Tile* ptile = mTiles[index];
            if ( ptile->getTexCoord() == tile.getTexCoord() )
            {
                return index;
            }
        }
    }
    return 255;
}

// - Painting

void TileMap::paint(QPainter& painter)
{
    const LayerDefinition& layerdef = mpLayer->getDefinition();

    int posy = 0;
    for ( int y = 0; y < layerdef.height; ++y, posy += mTileSet.getTileHeight() )
    {
        int posx = 0;
        for ( int x = 0; x < layerdef.width; ++x, posx += mTileSet.getTileWidth() )
        {
            for ( int level = 2; level >= 0; --level )
            {
                int tile = mpLayer->getTile((LayerLevel)level, x, y);
                if ( tile < 255 )
                {
                    Tile* ptile = mTiles[tile];
                    ptile->paint(painter, posx, posy);
                }
            }
        }
    }
}

void TileMap::paint(QPainter& painter, Tile& tile, int x, int y)
{
    const QPoint& texcoords = tile.getTexCoord();
    painter.drawImage(x, y, mImage, texcoords.x(), texcoords.y(), mTileSet.getTileWidth(), mTileSet.getTileHeight());
}

// - Operations

void TileMap::resize(const QSize& size)
{
    QSize currentsize(mpLayer->getDefinition().width, mpLayer->getDefinition().height);
    if ( size != currentsize )
    {
        mpLayer->resize(size.width(), size.height());

        int width = mTileSet.getTileWidth();
        int height = mTileSet.getTileHeight();

        mSize.setWidth(width * size.width());
        mSize.setHeight(height * size.height());

        mWorld.mapChanged(*this);
    }
}

void TileMap::generateTiles()
{
   int maxFramesPerRow = static_cast<int>(mImage.width() / mTileSet.getTileWidth());

   // build the texture coord lookup table
   for (int tc = 0; tc < mTileSet.getTileCount(); tc++)
   {
        // calculate starting texture coordinates
        int x = (tc % maxFramesPerRow) * mTileSet.getTileWidth();
        int y = (int) floorf ((float)tc / maxFramesPerRow) * mTileSet.getTileHeight();

        mTiles.append(new Tile(*this, QPoint(x, y)));
    }
}

Tile TileMap::getTile(const QPoint& mousepos, LayerLevel level)
{
    int tilex = mousepos.x() / mTileSet.getTileWidth();
    int tiley = mousepos.y() / mTileSet.getTileHeight();

    if ( tilex >= 0 && tilex < mpLayer->getDefinition().width
      && tiley >= 0 && tiley < mpLayer->getDefinition().height )
    {
        int tileindex = mpLayer->getTile(level, tilex, tiley);
        if ( tileindex < 255 )
        {
            return *mTiles[tileindex];
        }
    }

    return Tile();
}

bool TileMap::setTile(const QPoint& mousepos, LayerLevel level, const Tile& tile)
{
    int tilex = mousepos.x() / mTileSet.getTileWidth();
    int tiley = mousepos.y() / mTileSet.getTileHeight();

    if ( tilex >= 0 && tilex < mpLayer->getDefinition().width
      && tiley >= 0 && tiley < mpLayer->getDefinition().height )
    {
        int index = indexOf(tile);
        mpLayer->setTile(level, tilex, tiley, index);
        mWorld.mapChanged(*this);
        return true;
    }

    return false;
}

void TileMap::clearTile(const QPoint& mousepos, LayerLevel level)
{
    int tilex = mousepos.x() / mTileSet.getTileWidth();
    int tiley = mousepos.y() / mTileSet.getTileHeight();

    if ( tilex >= 0 && tilex < mpLayer->getDefinition().width
      && tiley >= 0 && tiley < mpLayer->getDefinition().height )
    {
        mpLayer->setTile(level, tilex, tiley, -1);
    }
}

// - Fixing

void TileMap::fix()
{
    const LayerDefinition& def = mpLayer->getDefinition();

    for ( int y = 0; y < def.height; ++y )
    {
        for ( int x = 0; x < def.width; ++x )
        {
            int tile = qMax(-1, mpLayer->getTile(LayerLevel::eMid, x, y) - 1);
            mpLayer->setTile(LayerLevel::eMid, x, y, tile);
        }
    }
}
