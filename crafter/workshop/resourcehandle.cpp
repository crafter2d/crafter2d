#include "resourcehandle.h"

#include "resource.h"

static const QString sEmpty;

ResourceHandle::ResourceHandle():
    QObject(nullptr),
    mpResource(nullptr)
{
}

ResourceHandle::ResourceHandle(const ResourceHandle& handle):
    QObject(nullptr),
    mpResource(handle.mpResource)
{
}

ResourceHandle::ResourceHandle(Resource &resource):
    QObject(nullptr),
    mpResource(&resource)
{
}

ResourceHandle& ResourceHandle::operator=(const ResourceHandle& that)
{
    mpResource = that.mpResource;
    return *this;
}

ResourceHandle& ResourceHandle::operator=(Resource& that)
{
    mpResource = &that;
    return *this;
}

const QString& ResourceHandle::getResourceName() const
{
    return mpResource != nullptr ? mpResource->getResourceName() : sEmpty;
}
