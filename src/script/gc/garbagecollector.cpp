
#include "garbagecollector.h"

#include "script/vm/virtualmachine.h"

GarbageCollector::GarbageCollector():
   mObjects()
{
}

// - Operations

void GarbageCollector::collect(VirtualObjectReference& object)
{
   mObjects.push_back(object);
}

void GarbageCollector::gc(VirtualMachine& vm)
{
   Objects::iterator it = mObjects.begin();
   for ( ; it != mObjects.end(); ++it )
   {
      VirtualObjectReference& object = (*it);
      if ( object->hasNativeObject() )
      {
         vm.unregisterNative(object);
      }
   }

   // the objects will delete the native objects they own themselfs

   mObjects.clear();
}
