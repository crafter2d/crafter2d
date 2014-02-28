#include "projecttreeworlditem.h"

#include <QFileInfo>

#include "../tileworld.h"

ProjectTreeWorldItem::ProjectTreeWorldItem(const TileWorldHandle& world) :
    ProjectTreeItem(ProjectTreeItem::eWorld),
    mWorld(world)
{
}

// - Get/set

TileWorld& ProjectTreeWorldItem::getWorld()
{
    return *mWorld;
}

QVariant ProjectTreeWorldItem::data()
{
    return QFileInfo(mWorld->getResourceName()).baseName();
}

QVariant ProjectTreeWorldItem::resourceData()
{
    return QVariant::fromValue(mWorld);
}
