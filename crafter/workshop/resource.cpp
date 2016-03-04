#include "resource.h"

Resource::Resource(Type type):
    QObject(nullptr),
    mType(type),
    mName(),
    mDirty(false)
{
}

Resource::Resource(const Resource& that):
    QObject(nullptr),
    mType(that.mType),
    mName(that.mName),
    mDirty(true)
{
}

Resource& Resource::operator=(const Resource& that)
{
    mType = that.mType;
    mName = that.mName;
    mDirty = true;
    return *this;
}

// - Get/set

const QString& Resource::getResourceName() const
{
    return mName;
}

void Resource::setResourceName(const QString& name)
{
    mName = name;
}

bool Resource::isDirty() const
{
    return mDirty;
}

void Resource::setDirty(bool dirty)
{
    mDirty = dirty;
}
