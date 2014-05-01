
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "core/core_base.h"
#include "core/string/string.h"

namespace c2d
{
   class CORE_API Exception
   {
   public:
      Exception();
      Exception(const String& reason);
      Exception(const Exception& that);
      
    // get/set
      const String& getReason() const;
      
   private:
     
    // data
       String mReason;
   };
}

#endif // EXCEPTION_H
