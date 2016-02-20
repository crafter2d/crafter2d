#ifndef TILESET_H
#define TILESET_H

#include "../resource.h"

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

    int getTileCount() const {
        return mTiles.size();
    }

  // operations
    void paintTile(QPainter& painter, const QPoint &pos, int index) const;

private:
    friend class TileSetWriter;
    friend class TileSetReader;

    struct Tile
    {
        QString name;
        int spriteindex;
    };

  // typedefs
    using Tiles = QVector<Tile>;

  // operations
    void determineTileSize();

  // data
    QString mImagePath;
    Tiles   mTiles;
    QSize   mTileSize;
};

#endif // TILESET_H
