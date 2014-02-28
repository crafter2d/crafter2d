#ifndef PROJECTTREEWORLDITEM_H
#define PROJECTTREEWORLDITEM_H

#include "projecttreeitem.h"

#include "../world/tileworldhandle.h"

class ProjectTreeWorldItem : public ProjectTreeItem
{
public:
    explicit ProjectTreeWorldItem(const TileWorldHandle& world);

  // get/set
    TileWorld& getWorld();

  // query
    virtual QVariant data() override;
    virtual QVariant resourceData() override;

signals:

public slots:

private:

  // data
    TileWorldHandle mWorld;
};

#endif // PROJECTTREEWORLDITEM_H
