#include "projecttreeobjectitem.h"

#include <QFileInfo>

ProjectTreeObjectItem::ProjectTreeObjectItem(const ResourceHandle& handle) :
    ProjectTreeItem(ProjectTreeItem::eObject),
    mHandle(handle)
{
}

// - Get/set

QVariant ProjectTreeObjectItem::data()
{
    const QString& resourcename = mHandle.getResourceName();
    return QFileInfo(resourcename).baseName();
}

QVariant ProjectTreeObjectItem::resourceData()
{
    return QVariant::fromValue(mHandle);
}
