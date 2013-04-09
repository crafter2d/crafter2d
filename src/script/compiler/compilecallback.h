
#ifndef COMPILE_CALLBACK_H_
#define COMPILE_CALLBACK_H_

#include "script/script_base.h"

namespace CIL
{
   class Class;
}

class SCRIPT_API CompileCallback
{
public:
   CompileCallback();

   virtual void notify(CIL::Class* pclass) = 0;
};

#endif // COMPILE_CALLBACK_H_
