#ifndef TILEWORLD_H
#define TILEWORLD_H

#include <QVector>

#include "../resource.h"

#include "tileworlddesc.h"
#include "tilefield.h"

class QSize;
class QPainter;

class Tile;
class TileBound;
class TileMap;

class TileWorld : public Resource
{
    Q_OBJECT

public:
    enum Layout { eTopDown, eIsoMetric };

    explicit TileWorld(const TileWorldDesc& desc);

  // query
    const TileWorldDesc& getDesc() const;

    const QString& getName() const;

    QSize getMinimumSize() const;

    bool     hasActiveMap() const;
    TileMap* getActiveMap();
    void     setActiveMap(TileMap& active);
    void     setActiveMap(TileMap *active);

    int            getMapCount() const;
    const TileMap& getMap(int index) const;
          TileMap& getMap(int index);

    bool       hasSelectedBound() const;
    TileBound& getSelectedBound();
    void       setSelectedBound(TileBound* pbound);

  // painting
    void paint(QPainter& painter);

  // maintenance
    void addMap(TileMap* pmap);
    void removeMap(TileMap& map);

    int getBoundCount() const;
    const TileBound& getBound(int index) const;
    void addBound(TileBound* pbound);
    TileBound& addBound(const QPoint& mousepos);

  // operations
    int  getTile(const QPoint& mousepos, TileField::Level level) const;
    bool setTile(const QPoint& mousepos, TileField::Level level, int tile);
    void clearTile(const QPoint& mousepos, TileField::Level level);

    void moveUp(int index);
    void moveDown(int index);

    void straightenBounds();

  // finding
    TileBound* findBound(const QPoint& mousepos);

signals:
    void activeMapChanged(TileMap* pmap);
    void worldDirty();

public slots:
    void on_mapChanged(TileMap& map);

private:
    typedef QVector<TileMap*> Maps;
    typedef QVector<TileBound*> Bounds;

    TileWorld(const TileWorld& that);

  // painting
    void paintMaps(QPainter& painter);
    void paintBounds(QPainter& painter);
    void paintSelectedBound(QPainter& painter);

  // data
    TileWorldDesc   mDesc;
    Maps            mMaps;
    Bounds          mBounds;
    TileMap*        mpActiveMap;
    TileBound*      mpSelectedBound;
};

#endif // TILEWORLD_H
