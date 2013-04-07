
#include "function.h"

namespace CIL
{

Function::Function():
    mName(),
    mArgs(),
    mLocals(),
    mFlags(0)
{
}

// - Operations

void Function::setName(const String& name)
{
   mName = name;
}

void Function::setInstructions(const CIL::Instructions& instructions)
{
   mInstructions = instructions;
}

void Function::setLocals(const CIL::Types& locals)
{
   mLocals = locals;
}

} // namespace CIL
