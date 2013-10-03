#ifndef TILEMAP_H
#define TILEMAP_H

#include <QImage>
#include <QPoint>
#include <QSize>
#include <QVector>

#include <engine/world/layertype.h>
#include <engine/world/tileset.h>

class QPainter;
class QSize;

class Layer;
class Tile;
class TileSet;
class TileWorld;

class TileMap
{
public:
    explicit TileMap(TileWorld& world, Layer* player);

  // query
    Layer& getLayer();

    const QString getName() const;
    const QSize &getSize() const;

    QSize getTileSize() const;
    int   getTileCount() const;
    Tile& getTile(int tile);

    int indexOf(const Tile& tile) const;

  // painting
    void paint(QPainter& painter);
    void paint(QPainter& painter, Tile& tile, int x, int y);

  // operations
    void load(const QString& path);

    bool setTile(const QPoint& mousepos, LayerLevel level, const Tile &tile);
    void clearTile(const QPoint& mousepos, LayerLevel level);

    void resize(const QSize& size);

  // fixing
    void fix();

private:
    typedef QVector<Tile*> Tiles;

  // operations
    void generateTiles();

  // data
    TileWorld&  mWorld;
    Layer*      mpLayer;
    QImage      mImage;
    QSize       mSize;
    TileSet     mTileSet;
    Tiles       mTiles;

};

#endif // TILEMAP_H
