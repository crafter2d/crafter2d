
#ifndef WEAK_COUNT_H
#define WEAK_COUNT_H

#include "core/defines.h"

#include "countbase.h"

class WeakCount
{
public:
   WeakCount():
      mpCounter(NULL)
   {
   }

   WeakCount(const WeakCount& that):
      mpCounter(that.mpCounter)
   {
      if ( mpCounter != NULL )
      {
         mpCounter->addWeakRef();
      }
   }

   WeakCount(CountBase* pcounter):
      mpCounter(pcounter)
   {
      if ( mpCounter != NULL )
      {
         mpCounter->addWeakRef();
      }
   }

   ~WeakCount()
   {
      if ( mpCounter != NULL )
      {
         mpCounter->releaseWeakRef();
      }
   }

private:
   friend class SharedCount;

   CountBase* mpCounter;
};


#endif // WEAK_COUNT_H
