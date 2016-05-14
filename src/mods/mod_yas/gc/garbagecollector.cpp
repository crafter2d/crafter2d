
#include "garbagecollector.h"

#include <algorithm>
#include <cstring>

#include "core/containers/listiterator.h"
#include "core/smartptr/scopedvalue.h"

#include "mod_yas/vm/virtualobject.h"
#include "mod_yas/vm/virtualmachine.h"

int VoidHash(void* pkey)
{
	unsigned char* keyPtr = (unsigned char*)pkey;
	if ( pkey == NULL )
		return 0;
	
	unsigned int hashOffset = 0;
	unsigned int keySize = std::strlen((char*)pkey);

	for ( unsigned int i = 0; i < keySize; i++)
	{
      hashOffset = (hashOffset ^ keyPtr[i] ^ (keyPtr[i] << 1) ^ (keyPtr[i] << 8)) + (keyPtr[i] << (keyPtr[i] % 7));
   }
	
	return hashOffset;
}

GarbageCollector::GarbageCollector():
   mCollectables(),
   mState(eIdle)
{
}

GarbageCollector::~GarbageCollector()
{
}

// - Operations

void GarbageCollector::collect(Collectable* pcollectable)
{
   mCollectables.push_back(pcollectable);
}

void GarbageCollector::gc(VirtualMachine& vm)
{
   if ( mState == eIdle )
   {
      ScopedValue<State> state(&mState, eRunning, eIdle);
      phaseMark(vm);
      phaseCollect(vm);
   }
}

void GarbageCollector::phaseMark(VirtualMachine& vm)
{
   vm.mark();
}

void GarbageCollector::phaseCollect(VirtualMachine& vm)
{
   auto end = std::remove_if(mCollectables.begin(), mCollectables.end(), [&vm](Collectable* pobject)
   {
      if ( pobject->isMarked() )
      {
         pobject->setMarked(false);
         return false;
      }
      else
      {
         pobject->finalize(vm);
         return true;
      }
   });
   
   mCollectables.erase(end, mCollectables.end());
}
