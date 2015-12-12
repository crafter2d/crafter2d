
#include "jitcontext.h"

JITContext::JITContext():
   mFunctions()
{
}

void JITContext::add(JITFunction* pfunction)
{
   mFunctions.push_back(pfunction);
}
