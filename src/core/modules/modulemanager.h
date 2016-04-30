
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
   class ModuleFile;
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
      ModuleManager();
      ~ModuleManager();

      ModuleManager(const ModuleManager& that) = delete;
      ModuleManager& operator=(const ModuleManager& that) = delete;

      // operations
      bool initialize();
      
      ModuleCollection filter(ModuleKind kind);

    // query
      Module* lookup(const Uuid& uuid);

   private:
    // types
      using ModuleFiles = std::vector<ModuleFile>;
      
    // maintenance
      void load(const String& filename);
      void add(Module* pmodule);
      void clear();

      static ModuleManager sInstance;

    // data
      ModuleFiles      mFiles;
      ModuleCollection mModules;
   };
}

#endif // MODULE_MANAGER_H
