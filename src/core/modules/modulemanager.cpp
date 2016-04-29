
#include "modulemanager.h"

#include <algorithm>

#include "core/defines.h"
#include "core/log/log.h"
#include "core/system/platform.h"
#include "core/system/exception.h"
#include "core/vfs/filesystem.h"

#include "module.h"

namespace c2d
{
   // static 
   Modules* Modules::create(Module* pmodule)
   {
      Modules* pmodules = new Modules;
      pmodules->count = 1;
      pmodules->modules = new Module*[1];
      pmodules->modules[0] = pmodule;
      return pmodules;
   }

   // static 
   Modules* Modules::create(std::initializer_list<Module*> modulelist)
   {
      Modules* pmodules = new Modules;
      pmodules->count = modulelist.size();
      pmodules->modules = new Module*[modulelist.size()];

      int index = 0;
      for ( auto pmodule : modulelist )
      {
         pmodules->modules[index++] = pmodule;
      }
      return pmodules;
   }

   // static 
   void Modules::free(Modules* pmodules)
   {
      for ( int index = 0; index < pmodules->count; ++index )
      {
         delete pmodules->modules[index];
      }
      delete[] pmodules->modules;
      delete pmodules;
   }

   ModuleManager::ModuleManager() :
      mModules()
   {
   }

   ModuleManager::~ModuleManager()
   {
      deinitialize();
   }

   // - Initialization

   bool ModuleManager::initialize()
   {
      std::vector<String> filenames;
      FileSystem& fs = FileSystem::getInstance();
   #if defined(WIN32)
      if ( !fs.find(UTEXT("mod_*.dll"), filenames, false) )
   #else
      if ( !fs.find(UTEXT("libmod_*.so"), filenames, false) )
   #endif
      {
         return false;
      }

      // load all modules
      Log::getInstance().info("Loading modules...");

      auto append = [this](const String& val){ add(val); };
      std::for_each(filenames.begin(), filenames.end(), append);

      return true;
   }

   void ModuleManager::deinitialize()
   {
      clear();

      
   }

   // - Query

   ModuleCollection ModuleManager::filter(ModuleKind kind)
   {
      return mModules.filter(kind);
   }

   // - Maintenance

   void ModuleManager::add(const String& filename)
   {
      Log::getInstance().info(UTEXT("Loading module ") + filename);
      
      Platform& platform = c2d::Platform::getInstance();
      void* pmodule = platform.loadModule(filename);
      if ( pmodule != NULL )
      {
         ModuleHandle handle;
         handle.phandle = pmodule;

         PGETMODULES pfunc = (PGETMODULES)platform.getFunctionAddress(pmodule, UTEXT("getModules"));
         if ( pfunc != NULL )
         {
            Modules* pmodules = (*pfunc)();
            if ( pmodules != nullptr )
            {
               for ( int index = 0; index < pmodules->count; index++ )
               {
                  auto pmodule = pmodules->modules[index];
                  add(pmodule);
                  handle.modules.push_back(pmodule);
               }
            }
         }

         mHandles.push_back(std::move(handle));
      }
   }

   void ModuleManager::add(Module* pmodule)
   {
      pmodule->setModuleManager(*this);
      mModules.add(pmodule);
   }

   void ModuleManager::clear()
   {
      mModules.clear();

      Platform& platform = Platform::getInstance();
      for ( auto& handle : mHandles )
      {
         platform.freeModule(handle.phandle);
      }
   }

   // - Query

   Module* ModuleManager::lookup(const Uuid& uuid)
   {
      return mModules[uuid];
   }
}
