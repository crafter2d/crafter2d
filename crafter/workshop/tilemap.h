#ifndef TILEMAP_H
#define TILEMAP_H

#include <QImage>
#include <QPoint>
#include <QSize>

#include "world/tilemapdesc.h"
#include "world/tilefield.h"

class QPainter;
class QSize;

class Tile;
class TileField;
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

    const TileField& getField() const;
          TileField& getField();

    const QSize& getMinimumSize() const;

    const QSize& getTileSize() const;
    int          getTileCount() const;

    const Tile& getTile(int tile);

    const QTileSet& getTileSet() const;

    int indexOf(const Tile& tile) const;

  // painting
    void paint(QPainter& painter);
    void paint(QPainter& painter, Tile& tile, int x, int y);

  // operations
    void setField(TileField* pfield);
    void setTileSet(QTileSet* ptileset);

    Tile getTile(const QPoint& mousepos, TileField::Level level) const;
    bool setTile(const QPoint& mousepos, TileField::Level level, const Tile &tile);
    void clearTile(const QPoint& mousepos, TileField::Level level);

    void resize(const QSize& size);

signals:
    void mapChanged(TileMap& layer);

private:
  // operations
    bool setTile(const QPoint &mousepos, TileField::Level level, int tileindex);

  // data
    TileMapDesc mDesc;
    TileField*  mpField;
    QTileSet*   mpTileSet;
    QSize       mPixelSize;
};

#endif // TILEMAP_H
