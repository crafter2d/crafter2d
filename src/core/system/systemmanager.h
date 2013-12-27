
#ifndef __SystemManager_h__
#define __SystemManager_h__

#include <map>

#include "core/core_base.h"

#include "systeminterface.h"

class ModuleManager;

namespace c2d
{
   class System;

   class CORE_API SystemManager
   {
   public:
      SystemManager();

    // query
      System& getSystem(SystemKind kind);

    // operations
      void initialize(ModuleManager& manager);

    // registration
      void registerSystem(System& psystem);

   private:
      typedef std::map<SystemKind, System*> SystemMap;

    // data
      SystemMap mSystems;
   };
}

#endif // __SystemManager_h__
