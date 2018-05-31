
#include "sharedcount.h"

WeakCount::WeakCount():
   mpCounter(nullptr)
{
}

WeakCount::WeakCount(const WeakCount& that):
   mpCounter(that.mpCounter)
{
   if ( mpCounter != nullptr )
   {
      mpCounter->addWeakRef();
   }
}

WeakCount::WeakCount(const SharedCount& that):
   mpCounter(that.mpCounter)
{
   if ( mpCounter != nullptr )
   {
      mpCounter->addWeakRef();
   }
}

WeakCount::~WeakCount()
{
   if ( mpCounter != nullptr )
   {
      mpCounter->releaseWeakRef();
   }
}
