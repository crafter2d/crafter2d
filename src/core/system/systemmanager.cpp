
#include "systemmanager.h"

#include "system.h"

namespace c2d
{
   // - Query

   System& SystemManager::getSystem(SystemKind kind)
   {
      SystemMap::iterator it = mSystems.find(kind);
      return *it->second;
   }

   // - Registration

   void SystemManager::registerSystem(System& system)
   {
      mSystems.insert(std::make_pair(system.getKind(), &system));
   }
}
