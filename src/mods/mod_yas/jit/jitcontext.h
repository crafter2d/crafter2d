
#ifndef JIT_CONTEXT_H
#define JIT_CONTEXT_H

#include <list>

class JITFunction;

class JITContext
{
   using Functions = std::list<JITFunction*>;

public:
   JITContext();

   void add(JITFunction* pfunction);

private:

   Functions mFunctions;
};

#endif // JIT_CONTEXT_H
