#include "resource.h"

Resource::Resource(Type type):
    QObject(nullptr),
    mType(type),
    mResourceName(),
    mDirty(false)
{
}

Resource::Resource(const Resource& that):
    QObject(nullptr),
    mType(that.mType),
    mResourceName(that.mResourceName),
    mDirty(true)
{
}

Resource& Resource::operator=(const Resource& that)
{
    mType = that.mType;
    mResourceName = that.mResourceName;
    mDirty = true;
    return *this;
}

// - Get/set

const QString& Resource::getResourceName() const
{
    return mResourceName;
}

void Resource::setResourceName(const QString& name)
{
    mResourceName = name;
}

bool Resource::isDirty() const
{
    return mDirty;
}

void Resource::setDirty(bool dirty)
{
    mDirty = dirty;
}
