
#include "compiledclass.h"

CompiledClass::CompiledClass():
   mFunctions()
{
}

// - Operations
   
void CompiledClass::addFunction(CompiledFunction* pfunction)
{
   mFunctions.push_back(pfunction);
}
