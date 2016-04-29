
#ifndef MODULE_COLLECTION_H
#define MODULE_COLLECTION_H

#include <map>

#include "module.h"

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
      using ModuleMap = std::map<Uuid, Module*>;

   public:
      ModuleCollection();
      ModuleCollection(ModuleCollection&& that);
      ~ModuleCollection();
      ModuleCollection& operator=(ModuleCollection&& that);
      ModuleCollection& operator=(const ModuleCollection& that);

      typedef ModuleMap::iterator iterator;

      iterator begin() { return mModules.begin(); }
      iterator end() { return mModules.end(); }
      
    // query
      Module* operator[](const Uuid& uuid);
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
