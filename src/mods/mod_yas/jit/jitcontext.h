
#ifndef JIT_CONTEXT_H
#define JIT_CONTEXT_H

#include "core/containers/list.h"

class JITFunction;

class JITContext
{
   typedef List<JITFunction*> Functions;

public:
   JITContext();

   void add(JITFunction* pfunction);

private:

   Functions mFunctions;
};

#endif // JIT_CONTEXT_H
