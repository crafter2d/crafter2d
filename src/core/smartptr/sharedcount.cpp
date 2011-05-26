
#include "sharedcount.h"

SharedCount::SharedCount():
   mpCounter(new int(0))
{
   (*mpCounter) = 1;
}

SharedCount::SharedCount(const SharedCount& that):
   mpCounter(that.mpCounter)
{
   inc();
}

SharedCount::~SharedCount()
{
   dec();

   if ( (*mpCounter) == 0 )
   {
      delete mpCounter;
      mpCounter = NULL;
   }
}

const SharedCount& SharedCount::operator=(const SharedCount& that)
{
   mpCounter = that.mpCounter;
   inc();

   return *this;
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

void SharedCount::inc()
{
   ASSERT_PTR(mpCounter);
   (*mpCounter)++;
}

void SharedCount::dec()
{
   ASSERT_PTR(mpCounter);
   (*mpCounter)--;
}
