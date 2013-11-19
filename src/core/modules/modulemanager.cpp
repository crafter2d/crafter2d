
#include "modulemanager.h"

#include <algorithm>

#include "core/content/contentmodule.h"
#include "core/smartptr/autoptr.h"
#include "core/system/platform.h"
#include "core/vfs/filesystem.h"

#include "module.h"

typedef Module* (*PGETMODULE)();
typedef ModuleCollection* (*PGETMODULECOLLECTION)();

ModuleManager::ModuleManager():
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
   if ( !fs.find(UTEXT("mod_*.dll"), filenames) )
   {
      return false;
   }

   // load all modules
   auto append = [this](const String& val){ add(val); };
   std::for_each(filenames.begin(), filenames.end(), append);

   return true;
}

void ModuleManager::deinitialize()
{
   clear();

   auto close = [](void* pmodule) { Platform::getInstance().freeModule(pmodule); };
   std::for_each(mHandles.begin(), mHandles.end(), close);
}

// - Query

void ModuleManager::getModules(ModuleCollection& modules)
{
   modules.add(mModules);
}

// - Maintenance

void ModuleManager::add(const String& filename)
{
   Platform& platform = Platform::getInstance();
   void* pmodule = platform.loadModule(filename);
   if ( pmodule != NULL )
   {
      mHandles.push_back(pmodule);

      PGETMODULE pfunc = (PGETMODULE) platform.getFunctionAddress(pmodule, UTEXT("getModule"));
      if ( pfunc != NULL )
      {
         AutoPtr<Module> module = (*pfunc)();
         if ( !module.hasPointer() )
         {
            throw std::exception("Could not get the module handle.");
         }

         mModules.add(module.release());
      }
      else
      {
         PGETMODULECOLLECTION pcolfunc = (PGETMODULECOLLECTION) platform.getFunctionAddress(pmodule, UTEXT("getModuleCollection"));
         if ( pcolfunc != NULL )
         {
            AutoPtr<ModuleCollection> collection = (*pcolfunc)();
            if ( collection.hasPointer() )
            {
               mModules.add(*collection);
            }
         }
      }
   }
}

void ModuleManager::clear()
{
   for ( std::size_t index = 0; index < mModules.size(); ++index )
   {
      Module* pmodule = &mModules[index];
      delete pmodule;
   }
   mModules.clear();
}
