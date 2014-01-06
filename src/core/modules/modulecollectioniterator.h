
#ifndef __ModuleCollectionIterator_h__
#define __ModuleCollectionIterator_h__

#include <map>

#include "core/system/uuid.h"
#include "core/core_base.h"

namespace c2d
{
   class Module;

   typedef std::map<Uuid, Module*> ModuleMap;

   class CORE_API ModuleCollectionIterator
   {
   public:
      ModuleCollectionIterator(const ModuleCollectionIterator& that);

      void operator=(const ModuleCollectionIterator& that);

    // iterator interface
      bool isValid() const;
      void operator++();
      Module& operator*();

   private:
      friend class ModuleCollection;

      ModuleCollectionIterator(ModuleMap::iterator current, ModuleMap::iterator end);

    // data
      ModuleMap::iterator mCurrent;
      ModuleMap::iterator mEnd;
   };
}

#endif // __ModuleCollectionIterator_h__
