#ifndef TILEMAP_H
#define TILEMAP_H

#include <QImage>
#include <QPoint>
#include <QSize>
#include <QVector>

#include "world/tilemapdesc.h"
#include "world/tilefield.h"

class QPainter;
class QSize;

class Tile;
class QTileField;
class QTileSet;

class TileMap : public QObject
{
    Q_OBJECT

public:
    TileMap(const TileMapDesc& desc);

  // query
    const QString& getName() const;

    const TileMapDesc& getDesc() const;
          TileMapDesc& getDesc();
    void               setDesc(const TileMapDesc& desc);

    const QTileField& getField() const;
          QTileField& getField();

    const QSize& getMinimumSize() const;

    const QSize& getTileSize() const;
    int          getTileCount() const;

    Tile& getTile(int tile);

    int indexOf(const Tile& tile) const;

  // painting
    void paint(QPainter& painter);
    void paint(QPainter& painter, Tile& tile, int x, int y);

  // operations
    void setField(QTileField* pfield);
    void setTileSet(QTileSet* ptileset);

    bool setTile(const QPoint& mousepos, QTileField::Level level, const Tile &tile);
    void clearTile(const QPoint& mousepos, QTileField::Level level);

    void resize(const QSize& size);

signals:
    void mapChanged(TileMap& layer);

private:
    typedef QVector<Tile*> Tiles;

  // operations
    void generateTiles();
    bool setTile(const QPoint &mousepos, QTileField::Level level, int tileindex);

  // data
    TileMapDesc mDesc;
    QTileField* mpField;
    QTileSet*   mpTileSet;
    QSize       mPixelSize;
    Tiles       mTiles;
};

#endif // TILEMAP_H
