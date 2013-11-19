
#ifndef MODULE_COLLECTION_H
#define MODULE_COLLECTION_H

#include <vector>

#include "core/core_base.h"

#include "module.h"

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
   ModuleCollection(Module::Type filter = Module::eInvalid);
   ~ModuleCollection();

 // operators
   Module& operator[](int index);
   ModuleCollection& operator=(const ModuleCollection& that);

 // query
   int size() const;

 // maintenance
   void add(Module* pmodule);
   void add(const ModuleCollection& collection);
   void clear();

private:
 // types
   typedef std::vector<Module*> Modules;

 // data
   Modules        mModules;
   Module::Type   mFilterType;
};

#endif // MODULE_COLLECTION_H
