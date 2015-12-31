
#include "core/defines.h"

CountBase::CountBase():
   mUseCount(1),
   mWeakCount(1)
{
}

CountBase::CountBase(const CountBase& that):
   mUseCount(that.mUseCount),
   mWeakCount(that.mWeakCount)
{
}

CountBase::~CountBase()
{
}

// query
   
INLINE int CountBase::useCount() const
{
   return mUseCount;
}

// - Operations

INLINE void CountBase::addRef()
{
   ++mUseCount;
}

INLINE void CountBase::releaseRef()
{
   --mUseCount;
   if ( mUseCount == 0 )
   {
      dispose();
      releaseWeakRef();
   }
}

INLINE void CountBase::addWeakRef()
{
   ++mWeakCount;
}

INLINE void CountBase::releaseWeakRef()
{
   if ( --mWeakCount == 0 )
   {
      delete this;
   }
}
