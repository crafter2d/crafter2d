#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QPoint>

class QTileSet;
class QPainter;

class Tile : public QObject
{
    Q_OBJECT

public:
    Tile();
    Tile(const Tile& that);
    Tile(QTileSet& tileset, QPoint& texcoord);

    Tile& operator=(const Tile& that);

  // get/set
    const QSize& getSize() const;
    const QPoint& getTexCoord() const;

  // qurey
    bool isValid() const;

  // painting
    void paint(QPainter &painter, int x, int y) const;

private:

  // data
    QTileSet* mpTileSet;
    QPoint   mTexCoord;
};

Q_DECLARE_METATYPE(Tile);

#endif // TILE_H
