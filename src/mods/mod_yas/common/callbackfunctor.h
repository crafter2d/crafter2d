
#ifndef __CallbackFunctor_h__
#define __CallbackFunctor_h__

#include "mod_yas/script_base.h"

class VirtualCall;

namespace yas
{
   class CallbackFunctor
   {
   public:
      
      virtual void exec(VirtualCall& call) = 0;

   };
}

#endif // __CallbackFunctor_h__
