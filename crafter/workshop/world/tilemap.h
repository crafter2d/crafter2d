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
class TileSet;

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

    const TileSet& getTileSet() const;
          TileSet& getTileSet();

    int indexOf(const Tile& tile) const;

  // painting
    void paint(QPainter& painter);

  // operations
    void setField(TileField* pfield);
    void setTileSet(TileSet* ptileset);

    int  getTile(const QPoint& mousepos, TileField::Level level) const;
    bool setTile(const QPoint& mousepos, TileField::Level level, int tile);
    void clearTile(const QPoint& mousepos, TileField::Level level);

    void resize(const QSize& size);

signals:
    void mapChanged(TileMap& layer);

private:

  // data
    TileMapDesc mDesc;
    TileField*  mpField;
    TileSet*    mpTileSet;
    QSize       mPixelSize;
};

#endif // TILEMAP_H
