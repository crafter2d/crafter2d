
#include "systemmanager.h"

#include "core/modules/modulemanager.h"
#include "core/modules/modulecollection.h"

#include "system.h"
#include "systemmodule.h"

namespace c2d
{
   SystemManager::SystemManager():
      mSystems()
   {
   }

   // - Query

   System& SystemManager::getSystem(SystemKind kind)
   {
      SystemMap::iterator it = mSystems.find(kind);
      return *it->second;
   }

   // - Operations

   void SystemManager::initialize(ModuleManager& manager)
   {
      ModuleCollection col(Module::eSystem);
      manager.getModules(col);

      for ( int index = 0; index < col.size(); ++index )
      {
         SystemModule& module = static_cast<SystemModule&>(col[index]);
         registerSystem(module.getSystem());
      }
   }

   // - Registration

   void SystemManager::registerSystem(System& system)
   {
      mSystems.insert(std::make_pair(system.getKind(), &system));
   }
}
