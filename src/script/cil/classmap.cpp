
#include "classmap.h"

#include "class.h"

namespace CIL
{

   void ClassMap::insert(Class* pclass)
   {
      mClassMap[pclass->getName()] = pclass;
   }

} // namespace CIL