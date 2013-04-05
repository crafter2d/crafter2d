
#include "compiledfunction.h"

CompiledFunction::CompiledFunction():
    mName(),
    mArgs(),
    mLocals(),
    mpRet(),
    mFlags(0)
{
}

// - Operations

void CompiledFunction::setName(const String& name)
{
   mName = name;
}

void CompiledFunction::setRetType(CIL::Type* ptype)
{
   mpRet = ptype;
}

void CompiledFunction::addArg(CIL::Type* ptype)
{
   mArgs.push_back(ptype);
}

void CompiledFunction::addLocal(CIL::Type* ptype)
{
   mLocals.push_back(ptype);
}
