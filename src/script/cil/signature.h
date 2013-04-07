
#ifndef CIL_SIGNATURE_H
#define CIL_SIGNATURE_H

#include "cil.h"

namespace CIL
{
   class Signature
   {
   public:
      Signature();

   private:
      Types mArguments;
      Type* mpReturn;
   };
}

#endif // CIL_SIGNATURE_H
