
#include "resourcehandlebase.h"

#include "core/defines.h"

#include "resourcemanager.h"

ResourceHandleBase::ResourceHandleBase(ResourceManager& manager):
   mManager(manager),
   mName()
{
}

ResourceHandleBase::~ResourceHandleBase()
{
}

// - Get/set
ResourceManager& ResourceHandleBase::getResourceManager()
{
   return mManager;
}

const String& ResourceHandleBase::getName() const
{
   return mName;
}

void ResourceHandleBase::setName(const String& name)
{
   mName = name;
}
// - Operations
   
void ResourceHandleBase::release()
{
   mManager.notifyResourceDeleted(*this);
}
