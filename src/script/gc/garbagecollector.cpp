
#include "garbagecollector.h"

#include "core/containers/hashmapiterator.h"

#include "script/vm/virtualmachine.h"
#include "script/vm/virtualobject.h"

int VoidHash(void* pkey)
{
	unsigned char* keyPtr = (unsigned char*)pkey;
	if ( pkey == NULL )
		return 0;
	
	unsigned int hashOffset = 0;
	unsigned int keySize = strlen((char*)pkey);

	for ( unsigned int i = 0; i < keySize; i++)
	{
      hashOffset = (hashOffset ^ keyPtr[i] ^ (keyPtr[i] << 1) ^ (keyPtr[i] << 8)) + (keyPtr[i] << (keyPtr[i] % 7));
   }
	
	return hashOffset;
}

GarbageCollector::GarbageCollector():
   mObjects()
{
   mObjects.setHashFunction(VoidHash);
}

GarbageCollector::~GarbageCollector()
{
   // the garbage collector should be empty before it is destructed
   ASSERT(mObjects.isEmpty());
}

// - Operations

void GarbageCollector::gc(VirtualMachine& vm)
{
   /*
   HashMapIterator<void*, VirtualObjectReference> it = mObjects.getIterator();
   while ( it.isValid() )
   {
      VirtualObjectReference ref = it.item();
      it.remove();

      ASSERT(!mObjects.contains(ref.ptr()));
      
      if ( isUnique(ref) )
      {
         if ( ref->hasNativeObject() )
         {
            vm.unregisterNative(*ref);
         }

         ref->collect(*this);
         it.reset();
      }
   }
   */
}
