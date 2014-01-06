
#include "modulecollectioniterator.h"

#include "core/defines.h"

namespace c2d
{
   ModuleCollectionIterator::ModuleCollectionIterator(ModuleMap::iterator current, ModuleMap::iterator end) :
      mCurrent(current),
      mEnd(end)
   {
   }

   ModuleCollectionIterator::ModuleCollectionIterator(const ModuleCollectionIterator& that):
      mCurrent(that.mCurrent),
      mEnd(that.mEnd)
   {
   }

   void ModuleCollectionIterator::operator=(const ModuleCollectionIterator& that)
   {
      mCurrent = that.mCurrent;
      mEnd = that.mEnd;
   }

   // - Iterator interface

   bool ModuleCollectionIterator::isValid() const
   {
      return mCurrent != mEnd;
   }

   void ModuleCollectionIterator::operator++()
   {
      mCurrent++;
   }

   Module& ModuleCollectionIterator::operator*()
   {
      ASSERT(isValid())
      return *mCurrent->second;
   }
}