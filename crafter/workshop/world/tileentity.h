#ifndef TILEENTITY_H
#define TILEENTITY_H

#include <QPoint>

class QPainter;

class Entity;
class TileWorld;

class TileEntity
{
public:
    explicit TileEntity(TileWorld& world, Entity& entity);

  // get/set
    const Entity& getEntity() const { return mDefinition; }

    const QPoint& getPosition() const { return mPosition; }
    void setPosition(const QPoint& pos) { mPosition = pos; }

    float getScale() const { return mScale; }
    void setScale(float scale) { mScale = scale; }

  // query
    QPoint getCenterPos() const;
    void setCenterPosition(const QPoint& pos);

  // operations
    bool hitTest(const QPoint& pos);
    void move(const QPoint& offset) { mPosition += offset; }

  // painting
    void paint(QPainter& painter);

private:

    TileWorld&  mWorld;
    Entity&     mDefinition;
    QPoint      mPosition;
    float       mScale;
};

#endif // TILEENTITY_H
