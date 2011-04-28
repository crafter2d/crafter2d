
#include "core/defines.h"

SharedCount::SharedCount():
   mpCounter(new int)
{
   (*mpCounter) = 1;
}

SharedCount::SharedCount(const SharedCount& that):
   mpCounter(that.mpCounter)
{
   ASSERT_PTR(mpCounter);
}

SharedCount::~SharedCount()
{
   ASSERT_PTR(mpCounter);
}

// - Query
   
bool SharedCount::isUnique() const
{
   ASSERT_PTR(mpCounter);
   return (*mpCounter) == 1;
}

// - Operations

void SharedCount::reset()
{
   ASSERT_PTR(mpCounter);
   (*mpCounter) = 0;
}