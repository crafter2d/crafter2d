#ifndef TILE_H
#define TILE_H

#include <QPoint>

#include "tilemap.h"

class QPainter;

class Tile : public QObject
{
    Q_OBJECT

public:
    Tile();
    Tile(const Tile& that);
    Tile(TileMap& map, QPoint& texcoord);

    Tile& operator=(const Tile& that);

  // get/set
    QSize getSize() const;
    const QPoint& getTexCoord() const;

  // painting
    void paint(QPainter &painter, int x, int y);

private:

  // data
    TileMap* mpMap;
    QPoint   mTexCoord;
};

Q_DECLARE_METATYPE(Tile)

#endif // TILE_H
