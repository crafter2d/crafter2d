
#include "guard.h"

#include <string.h>

namespace CIL
{
   Guard* Guard::clone() const
   {
      Guard* pguard = new Guard();
      pguard->finalize = finalize;
      memcpy(pguard->labels, labels, sizeof(int) * 4);
      return pguard;
   }
}
