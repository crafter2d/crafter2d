
#include "guard.h"

#include <cstring>

namespace CIL
{
   Guard* Guard::clone() const
   {
      Guard* pguard = new Guard();
      pguard->finalize = finalize;
      std::memcpy(pguard->labels, labels, sizeof(int) * 4);
      return pguard;
   }
}
