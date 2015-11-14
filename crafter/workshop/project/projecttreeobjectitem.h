#ifndef ProjectTreeObjectItem_H
#define ProjectTreeObjectItem_H

#include "projecttreeitem.h"

#include "../resourcehandle.h"

class ProjectTreeObjectItem : public ProjectTreeItem
{
public:
    explicit ProjectTreeObjectItem(const ResourceHandle& handle);

  // query
    virtual QVariant data() override;
    virtual QVariant resourceData() override;

private:

  // data
    ResourceHandle mHandle;
};

#endif // ProjectTreeObjectItem_H
