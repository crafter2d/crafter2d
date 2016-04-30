
#include "moduleinfo.h"

#include "core/system/uuid.h"

namespace c2d
{
   ModuleInfo * ModuleInfo::create(int version, std::initializer_list<Uuid> list)
   {
      auto pinfo = new ModuleInfo;
      pinfo->version = version;
      pinfo->numDependencies = list.size();
      pinfo->dependencies = new Uuid[list.size()];
   
      int index = 0;
      for ( const Uuid& uuid : list )
      {
         pinfo->dependencies[index++] = uuid;
      }
      return pinfo;
   }

   void ModuleInfo::free(ModuleInfo* pinfo)
   {
      delete[] pinfo->dependencies;
      delete pinfo;
   }
}
