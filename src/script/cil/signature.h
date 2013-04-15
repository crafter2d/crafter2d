
#ifndef CIL_SIGNATURE_H
#define CIL_SIGNATURE_H

#include "cil.h"

namespace CIL
{
   class Signature
   {
   public:

      const Type& operator[](int index) const;

    // query
      int size() const;
      bool match(const Signature& that) const;

    // operations
      void addArgument(Type* parg);

   private:
      
      Types mArguments;
   };
}

#endif // CIL_SIGNATURE_H
