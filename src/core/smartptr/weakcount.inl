
#include "sharedcount.h"

WeakCount::WeakCount():
   mpCounter(NULL)
{
}

WeakCount::WeakCount(const WeakCount& that):
   mpCounter(that.mpCounter)
{
   if ( mpCounter != NULL )
   {
      mpCounter->addWeakRef();
   }
}

WeakCount::WeakCount(const SharedCount& that):
   mpCounter(that.mpCounter)
{
   if ( mpCounter != NULL )
   {
      mpCounter->addWeakRef();
   }
}

WeakCount::~WeakCount()
{
   if ( mpCounter != NULL )
   {
      mpCounter->releaseWeakRef();
   }
}
