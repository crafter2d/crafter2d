
#ifndef SHARED_COUNT_H_
#define SHARED_COUNT_H_

#include "core/core_base.h"

class CORE_API SharedCount
{
public:
   SharedCount();
   SharedCount(const SharedCount& that);
   ~SharedCount();

   const SharedCount& operator=(const SharedCount& that);

 // query
   bool isUnique() const;

 // operations
   void reset();

private:
 // operations
   void inc();
   void dec();

   int*  mpCounter;
};

#include "sharedcount.inl"

#endif // SHARED_COUNT_H_
