
#include "garbagecollector.h"

#include "core/containers/listiterator.h"

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
   mCollectables()
{
}

GarbageCollector::~GarbageCollector()
{
}

// - Operations

void GarbageCollector::collect(Collectable* pcollectable)
{
   mCollectables.addTail(pcollectable);
}

void GarbageCollector::gc(VirtualMachine& vm)
{
   phaseMark(vm);
   phaseCollect(vm);
}

void GarbageCollector::phaseMark(VirtualMachine& vm)
{
   vm.mContext.mClassTable.mark();  // marks all statics
   vm.mStack.mark();

   for ( int index = 0; index < vm.mRootObjects.size(); index++ )
   {
      VirtualObject* pobject = vm.mRootObjects[index];
      pobject->mark();
   }
}

void GarbageCollector::phaseCollect(VirtualMachine& vm)
{
   ListIterator<Collectable*> it = mCollectables.getFront();
   while ( it.isValid() )
   {
      Collectable* pobject = *it;
      if ( pobject->isMarked() )
      {
         pobject->setMarked(false);

         ++it;
      }
      else
      {
         it.remove();

         pobject->finalize(vm);
         delete pobject;
      }
   }
}
