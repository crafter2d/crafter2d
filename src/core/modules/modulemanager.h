
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
      typedef Module* (*PGETMODULE)();
      typedef ModuleCollection* (*PGETMODULECOLLECTION)();

      static ModuleManager& getInstance();

      ~ModuleManager();

      // operations
      bool initialize();
      void deinitialize();

      void exec(PGETMODULE pfunc);
      void exec(PGETMODULECOLLECTION pfunc);
      
      ModuleCollection filter(ModuleKind kind);

    // query
      Module* lookup(const Uuid& uuid);

   private:
      // types
      typedef std::vector<void*> ModuleHandles;

      ModuleManager();

      // maintenance
      void add(Module* pmodule);
      void add(ModuleCollection& collection);
      void add(const String& filename);
      void clear();

      static ModuleManager sInstance;

      // data
      ModuleHandles    mHandles;
      ModuleCollection mModules;
   };
}

#endif // MODULE_MANAGER_H
