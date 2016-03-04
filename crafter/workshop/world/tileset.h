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

    struct Tile
    {
        QString name;
        QPoint offset;
        int spriteindex;
    };

    TileSet();
    TileSet(const TileSet& that);

    TileSet& operator=(TileSet&& that);

    Tile& operator[](int index) {
        return mTiles[index];
    }

    const Tile& operator[](int index) const {
        return mTiles[index];
    }

  // get/set
    const QSize& getTileSize() const {
        return mTileSize;
    }

    int getTileCount() const {
        return mTiles.size();
    }

  // Query
    bool empty() const;
    bool contains(const QString& name) const;
    QString getPath() const;
    QString getFullPath() const;

  // operations
    void add(const QString& name, int spriteindex);
    void clear();
    void update();

    void paintTile(QPainter& painter, const QPoint &pos, int index) const;

signals:
    void updated();

private:
    friend class TileSetWriter;
    friend class TileSetReader;

  // typedefs
    using Tiles = QVector<Tile>;

  // data
    Tiles   mTiles;
    QSize   mTileSize;
};

#endif // TILESET_H
