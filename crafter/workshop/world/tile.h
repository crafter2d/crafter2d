#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QPoint>

class Tile : public QObject
{
    Q_OBJECT

public:
             Tile();
             Tile(const Tile& that);
    explicit Tile(QPoint& texcoord);
             Tile(int x, int y);

    bool operator==(const Tile& that) const;
    bool operator!=(const Tile& that) const;

    Tile& operator=(const Tile& that);
    Tile& operator=(Tile&& that);

  // get/set
    const QPoint& getTexCoord() const;

  // qurey
    bool isValid() const;

private:

  // data
    QPoint   mTexCoord;
};

Q_DECLARE_METATYPE(Tile);

#endif // TILE_H
