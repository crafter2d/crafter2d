#ifndef TILESET_H
#define TILESET_H

#include "../resource.h"

#include <QImage>
#include <QSize>
#include <QVector>

class QPainter;
class Tile;

class TileSet : public Resource
{
    Q_OBJECT

public:
    static const int INVALID_TILE = 255;

    TileSet();


  // get/set
    const QString& getImagePath() const {
        return mImagePath;
    }

    void setImagePath(const QString& path);

    const QSize& getTileSize() const {
        return mTileSize;
    }

    int  getTileCount() const {
        return mTiles.size();
    }

  // operations
    void paintTile(QPainter& painter, int index, int x, int y) const;

private:

  // typedefs
    using QImages = QVector<QImage>;

  // data
    QString mImagePath;
    QImages mTiles;
    QSize   mTileSize;
};

#endif // TILESET_H
