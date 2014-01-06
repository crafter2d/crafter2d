
#ifndef MODULE_COLLECTION_H
#define MODULE_COLLECTION_H

#include "module.h"
#include "modulecollectioniterator.h"

namespace c2d
{
   /**
    * ModuleCollection is a collections of Modules. Those can be filtered by module
    * type. The container does not own the modules and it is the responsibility of
    * the owner of this container to delete the modules.
    *
    * @see ModuleCollection
    */

   class CORE_API ModuleCollection
   {
   public:
      ModuleCollection();
      ~ModuleCollection();

    // operators
      const Module* operator[](const Uuid& uuid) const;
            Module* operator[](const Uuid& uuid);

      ModuleCollectionIterator getIterator();

      ModuleCollection& operator=(const ModuleCollection& that);

    // query
      int size() const;

    // maintenance
      void add(Module* pmodule);
      void add(const ModuleCollection& collection);
      void clear();

    // filtering
      ModuleCollection filter(ModuleKind kind);

   private:

    // data
      ModuleMap      mModules;
   };
}

#endif // MODULE_COLLECTION_H
