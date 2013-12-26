
#include "system.h"

namespace c2d
{

   System::System(SystemKind kind) :
      mKind(kind)
   {
   }

   SystemKind System::getKind() const
   {
      return mKind;
   }
}