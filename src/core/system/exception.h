
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "core/string/string.h"

namespace c2d
{
   class Exception
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
