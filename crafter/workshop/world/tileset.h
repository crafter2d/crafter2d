#ifndef TILESET_H
#define TILESET_H

#include "../resource.h"

#include <QImage>
#include <QSize>
#include <QVector>

class QPainter;
class Tile;

class QTileSet : public Resource
{
    Q_OBJECT

public:
    QTileSet();

    const Tile& operator[](int index) const;

  // get/set
    bool          hasTexture() const;
    const QImage& getTexture() const;
    void          setTexture(const QImage& texture);

    const QString& getTileMap() const;
    void           setTileMap(const QString& map, bool forceReload = false);

    const QSize& getTileSize() const;
    void         setTileSize(const QSize& size);

    int  getTileCount() const;
    void setTileCount(int count);

  // query
    int indexOf(const Tile& item) const;

  // operations
    void paintTile(QPainter& painter, int index, int x, int y) const;

private:

  // typedefs
    typedef QVector<Tile> QTiles;

  // operations
    void loadTexture();
    void generateTiles();

  // data
    QString mTileMap;
    QImage  mTexture;
    QTiles  mTiles;
    QSize   mTileSize;
    int     mTileCount;
};

#endif // TILESET_H
