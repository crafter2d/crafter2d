
#include "jitmethodbuilder.h"

#include "core/defines.h"

struct OpcodeDesc
{

};

JITMethodBuilder::JITMethodBuilder(JITFunction& function):
   mFunction(function)
{
}

// - Builders

JITValue* JITMethodBuilder::addMul(JITValue* value1, JITValue* value2)
{
   return applyArith(value1, value2, false, false, false);
}

// - Helpers

JITValue* JITMethodBuilder::applyArith(JITValue* value1, JITValue* value2, bool intonly, bool floatonly, bool overflowcheck)
{
   return nullptr;
}
