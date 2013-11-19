
#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include "core/core_base.h"

#include "modulecollection.h"

class String;
class Module;

class CORE_API ModuleManager
{
public:
   ModuleManager();
   ~ModuleManager();

 // operations
   bool initialize();
   void deinitialize();

   void getModules(ModuleCollection& modules);

private:
 // types
   typedef std::vector<void*> ModuleHandles;

 // maintenance
   void add(const String& filename);
   void clear();

 // data
   ModuleHandles    mHandles;
   ModuleCollection mModules;
};

#endif // MODULE_MANAGER_H
