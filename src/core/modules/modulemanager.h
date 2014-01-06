
#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <vector>

#include "core/core_base.h"

#include "modulecollection.h"

class String;
class Module;

namespace c2d
{
   class Uuid;

   class CORE_API ModuleManager
   {
   public:
      ModuleManager();
      ~ModuleManager();

      // operations
      bool initialize();
      void deinitialize();

      ModuleCollection filter(ModuleKind kind);

    // query
      Module* lookup(const Uuid& uuid);

   private:
      // types
      typedef std::vector<void*> ModuleHandles;

      // maintenance
      void add(Module* pmodule);
      void add(ModuleCollection& collection);
      void add(const String& filename);
      void clear();

      // data
      ModuleHandles    mHandles;
      ModuleCollection mModules;
   };
}

#endif // MODULE_MANAGER_H
