
#include "modulemanager.h"

#include <algorithm>

#include "core/defines.h"
#include "core/log/log.h"
#include "core/system/platform.h"
#include "core/system/exception.h"
#include "core/vfs/filesystem.h"

#include "module.h"
#include "modulefile.h"

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
      mFiles(),
      mModules()
   {
   }

   ModuleManager::~ModuleManager()
   {
      clear();
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

      auto append = [this](const String& val){ load(val); };
      std::for_each(filenames.begin(), filenames.end(), append);

      using FilePtrs = std::vector<ModuleFile*>;
      FilePtrs files;
      for ( auto& file : mFiles )
      {
         files.push_back(&file);
      }

      FilePtrs::iterator it = files.begin();
      while ( !files.empty() )
      {
         ModuleFile& file = **it;
         const ModuleInfo& info = file.getInfo();

         bool mayLoad = true;
         if ( info.numDependencies > 0 )
         {
            for ( int index = 0; index < info.numDependencies; ++index )
            {
               const Uuid& uuid = info.dependencies[index];
               if ( mModules[uuid] == nullptr )
               {
                  mayLoad = false;
                  break;
               }
            }
         }

         if ( mayLoad )
         {
            file.load();
            Modules& modules = file.getModules();
            for ( int index = 0; index < modules.count; index++ )
            {
               auto pmodule = modules.modules[index];
               add(pmodule);
            }

            files.erase(it);
            it = files.begin();
         }
         else
         {
            it++;
         }

         if ( it == files.end() && !files.empty() )
         {
            Log::getInstance().error("Circular dependency detected while trying to load the modules.");
            break;
         }
      }

      return true;
   }
   
   // - Query

   ModuleCollection ModuleManager::filter(ModuleKind kind)
   {
      return mModules.filter(kind);
   }

   // - Maintenance

   void ModuleManager::load(const String& filename)
   {
      Log::getInstance().info(UTEXT("Loading module ") + filename);

      ModuleFile file(filename);
      if ( file.isValid() )
      {
         mFiles.push_back(std::move(file));
      }
      else
      {
         Log::getInstance().warning(UTEXT("Could not successfully load module ") + filename);
      }
   }

   void ModuleManager::add(Module* pmodule)
   {
      pmodule->setModuleManager(*this);
      mModules.add(pmodule);

      pmodule->initialize();
   }

   void ModuleManager::clear()
   {
      mModules.clear();
      mFiles.clear();
   }

   // - Query

   Module* ModuleManager::lookup(const Uuid& uuid)
   {
      return mModules[uuid];
   }
}
