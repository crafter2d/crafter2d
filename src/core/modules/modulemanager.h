
#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <vector>

#include "core/core_base.h"

#include "modulecollection.h"
#include "moduleinfo.h"

class String;
class Module;

namespace c2d
{
   class Uuid;

   struct CORE_API Modules
   {
      static Modules* create(Module* pmodule);
      static Modules* create(std::initializer_list<Module*> modulelist);
      static void     free(Modules* pmodules);

      int      count;
      Module** modules;
   };

   class CORE_API ModuleManager
   {
   public:
      typedef ModuleInfo* (*PGETMODULEINFO)();
      typedef Modules* (*PGETMODULES)();
      typedef void     (*PFREEMODULES)();

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
      struct ModuleHandle
      {
         std::vector<Module*> modules;
         void* phandle;
      };

      using ModuleHandles = std::vector<ModuleHandle>;
      
    // maintenance
      void add(Module* pmodule);
      void add(const String& filename);
      void clear();

      static ModuleManager sInstance;

    // data
      ModuleHandles    mHandles;
      ModuleCollection mModules;
   };
}

#endif // MODULE_MANAGER_H
