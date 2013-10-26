#ifndef TILEWORLD_H
#define TILEWORLD_H

#include <QObject>
#include <QVector>

#include <engine/world/layertype.h>

class QSize;
class QPainter;

class Bound;
class World;

struct LayerDefinition;

class Tile;
class TileMap;

class TileWorld : public QObject
{
    Q_OBJECT

public:
    static TileWorld* fromWorld(World *pworld);

    enum Layout { eTopDown, eIsoMetric };

    TileWorld();

  // query
    QString getName() const;

    QString getFileName() const;
    void setFileName(const QString& filename);

    QSize getMinimumSize() const;

    TileMap& getActiveMap();
    void     setActiveMap(TileMap& active);

    int      getMapCount() const;
    TileMap& getMap(int index);

    bool   hasSelectedBound() const;
    Bound& getSelectedBound();
    void   setSelectedBound(Bound* pbound);

  // painting
    void paint(QPainter& painter);

  // maintenance
    void addMap(TileMap* pmap);
    void addMap(LayerDefinition* pdefinition);

    Bound& addBound(const QPoint& mousepos);

  // operations
    Tile getTile(const QPoint& mousepos, LayerLevel level);
    bool setTile(const QPoint& mousepos, LayerLevel level, const Tile &tile);
    void clearTile(const QPoint& mousepos, LayerLevel level);

    void moveUp(int index);
    void moveDown(int index);

    void straightenBounds();

    void save();

  // finding
    Bound* findBound(const QPoint& mousepos);

  // fixing
    void fixMaps();

signals:
    void activeMapChanged(TileMap& map);
    void worldDirty();

public slots:
    void mapChanged(TileMap& map);

protected:
  // constructors
    explicit TileWorld(World* pworld);

private:
    typedef QVector<TileMap*> Maps;

    TileWorld(const TileWorld& that);

  // painting
    void paintMaps(QPainter& painter);
    void paintBounds(QPainter& painter);
    void paintSelectedBound(QPainter& painter);

  // data
    World*   mpWorld;
    Maps     mMaps;
    TileMap* mpActiveMap;
    Bound*   mpSelectedBound;
};

#endif // TILEWORLD_H
