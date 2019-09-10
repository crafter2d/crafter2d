#include "tileentity.h"

#include <QPainter>

#include "entity/entity.h"
#include "entity/spritecomponent.h"
#include "project.h"
#include "texturepacker/spriteatlas.h"

TileEntity::TileEntity(TileWorld& world, Entity& entity):
    mWorld(world),
    mDefinition(entity),
    mPosition(),
    mScale(1.0f)
{

}

// - Query

QPoint TileEntity::getCenterPos() const
{
    auto psprite = mDefinition.component<SpriteComponent>();
    if ( psprite )
    {
        QSize halfSize = psprite->getSize() / 2.0;
        return QPoint(mPosition.x() + halfSize.width(), mPosition.y() + halfSize.height());
    }
    return mPosition;
}

void TileEntity::setCenterPosition(const QPoint& pos)
{
    auto psprite = mDefinition.component<SpriteComponent>();
    if ( psprite )
    {
        QSize halfSize = psprite->getSize() / 2.0;
        mPosition.setX(pos.x() - halfSize.width());
        mPosition.setY(pos.y() - halfSize.height());
    }
    else
        mPosition = pos;
}

// - Operations

bool TileEntity::hitTest(const QPoint& pos)
{
    auto psprite = mDefinition.component<SpriteComponent>();
    if ( !psprite )
        return false;

    const QSize& size = psprite->getSize();
    QRect bounds(mPosition, size);
    return bounds.contains(pos);
}

// - Painting

void TileEntity::paint(QPainter& painter)
{
    auto psprite = mDefinition.component<SpriteComponent>();
    if ( psprite && !psprite->getTexture().isEmpty() )
    {
        auto& atlas = Project::getActiveProject().getSpriteAtlas();
        int index = atlas.lookup(psprite->getTexture());
        atlas.paint(painter, mPosition, index, mScale);
    }
}
