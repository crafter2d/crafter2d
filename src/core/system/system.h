
#ifndef __System_h__
#define __System_h__

#include "core/core_base.h"

#include "systeminterface.h"

namespace c2d
{
   class CORE_API System
   {
   public:
    // query
      SystemKind getKind() const;

   protected:
      System(SystemKind kind);

   private:

    // data
      SystemKind mKind;
   };
}

#endif // __System_h__
