
#ifndef WEAK_COUNT_H
#define WEAK_COUNT_H

#include "countbase.h"

class SharedCount;

class WeakCount
{
   WeakCount();
   WeakCount(const WeakCount& that);
   WeakCount(const SharedCount& that);
   ~WeakCount();

private:
   CountBase* mpCounter;
};

#endif // WEAK_COUNT_H
