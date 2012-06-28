
#ifndef WEAK_COUNT_H
#define WEAK_COUNT_H

#include "core/defines.h"

#include "countbase.h"
#include "sharedcount.h"

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

   WeakCount(const SharedCount& that):
      mpCounter(that.mpCounter)
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
   CountBase* mpCounter;
};


#endif // WEAK_COUNT_H
