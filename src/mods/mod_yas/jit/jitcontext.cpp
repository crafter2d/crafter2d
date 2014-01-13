
#include "jitcontext.h"

JITContext::JITContext():
   mFunctions()
{
}

void JITContext::add(JITFunction* pfunction)
{
   mFunctions.addTail(pfunction);
}
