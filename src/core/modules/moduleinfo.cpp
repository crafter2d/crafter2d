
#include "moduleinfo.h"

#include "core/system/uuid.h"

namespace c2d
{

ModuleInfo::ModuleInfo():
   mpDependencies(nullptr),
   mDepCount(0),
   mVersion(0)
{
}

void ModuleInfo::setDependencies(std::initializer_list<Uuid> list)
{
   int index = 0;
   mpDependencies = new Uuid[list.size()];
   for ( const Uuid& uuid : list )
   {
      mpDependencies[index++] = uuid;
   }
}

}
