
#include "callinterfacepatch.h"

CallInterfacePatch::CallInterfacePatch(ASTClass& klass, const ASTFunction& function):
   CodePatch(CodePatch::eCallInterface),
   mClass(klass),
   mFunction(function)
{
}

ASTClass& CallInterfacePatch::getClass()
{
   return mClass;
}

const ASTFunction&  CallInterfacePatch::getFunction()
{
   return mFunction;
}