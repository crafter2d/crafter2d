
#ifndef COUNT_BASE_H
#define COUNT_BASE_H

#include "core/core_base.h"

class CORE_API CountBase
{
public:
   CountBase();
   CountBase(const CountBase& that);

 // query
   int useCount() const;

 // operations
   void addRef();
   void releaseRef();

   void addWeakRef();
   void releaseWeakRef();

protected:
 // overloads
   virtual void dispose() = 0;

private:

   int mUseCount;
   int mWeakCount;
};

#ifdef JENGINE_INLINE
#  include "countbase.inl"
#endif

#endif // COUNT_BASE_H
