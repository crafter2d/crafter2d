
#ifndef RESOURCE_HANDLE_BASE_H
#define RESOURCE_HANDLE_BASE_H

#include "core/smartptr/refcount.h"
#include "core/string/string.h"

class ResourceManager;

class CORE_API ResourceHandleBase : public RefCount
{
public:
   explicit ResourceHandleBase(ResourceManager& manager);
   virtual ~ResourceHandleBase();

 // get/set
   ResourceManager& getResourceManager();

   const String& getName() const;
   void          setName(const String& name);

protected:
 // operations
   virtual void release() = 0;

private:
   ResourceManager&  mManager;
   String            mName;
};

#endif // RESOURCE_HANDLE_BASE_H
