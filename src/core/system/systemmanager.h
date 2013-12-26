
#ifndef __SystemManager_h__
#define __SystemManager_h__

#include <map>

#include "systeminterface.h"

namespace c2d
{
   class System;

   class SystemManager
   {
   public:
    // query
      System& getSystem(SystemKind kind);

    // registration
      void registerSystem(System& psystem);

   private:
      typedef std::map<SystemKind, System*> SystemMap;

    // data
      SystemMap mSystems;
   };
}

#endif // __SystemManager_h__
