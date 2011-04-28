
#ifndef SHARED_COUNT_H_
#define SHARED_COUNT_H_

class SharedCount
{
public:
   SharedCount();
   SharedCount(const SharedCount& that);
   ~SharedCount();

 // query
   bool isUnique() const;

 // operations
   void reset();

private:
   void init();

   int*  mpCounter;
};

#include "sharedcount.inl"

#endif // SHARED_COUNT_H_
