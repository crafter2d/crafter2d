
#ifndef MODULE_INFO_H
#define MODULE_INFO_H

#include "core/core_base.h"
#include <initializer_list>

namespace c2d
{
   class Uuid;

   struct CORE_API ModuleInfo
   {
      static ModuleInfo* create(int version, std::initializer_list<Uuid> list);
      static void        free(ModuleInfo* pinfo);
      
      Uuid* dependencies;
      int   numDependencies;
      int   version;
   };
}

#endif // MODULE_INFO_H

