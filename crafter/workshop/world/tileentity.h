#ifndef TILEENTITY_H
#define TILEENTITY_H

#include <QPoint>

class QRect;
class QPainter;

class Entity;
class TileMap;
class TileWorld;

class TileEntity
{
public:
    explicit TileEntity(TileWorld& world, Entity& entity);

  // get/set
    const Entity& getEntity() const { return mDefinition; }

    const TileMap* tileMap() const { return mpMap; }
    void setTileMap(TileMap* pmap) { mpMap = pmap; }

    const QPoint& getPosition() const { return mPosition; }
    void setPosition(const QPoint& pos) { mPosition = pos; }

    float getScale() const { return mScale; }
    void setScale(float scale) { mScale = scale; }

    int sortOrder() const { return mSortOrder; }
    void setSortOrder(int order) { mSortOrder = order; }

  // query
    QPoint getCenterPos() const;
    QRect getBoundingRect() const;
    void setCenterPosition(const QPoint& pos);

  // operations
    bool hitTest(const QPoint& pos);
    void move(const QPoint& offset) { mPosition += offset; }

  // painting
    void paint(QPainter& painter);

private:

    TileWorld&  mWorld;
    TileMap*    mpMap;
    Entity&     mDefinition;
    QPoint      mPosition;
    float       mScale;
    int         mSortOrder;
};

#endif // TILEENTITY_H
