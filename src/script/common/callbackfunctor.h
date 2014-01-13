
#ifndef __CallbackFunctor_h__
#define __CallbackFunctor_h__

#include "script/script_base.h"

class VirtualCall;

namespace yas
{
   class SCRIPT_API CallbackFunctor
   {
   public:
      
      virtual void exec(VirtualCall& call) = 0;

   };
}

#endif // __CallbackFunctor_h__
