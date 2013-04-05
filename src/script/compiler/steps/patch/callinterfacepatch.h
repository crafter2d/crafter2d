
#ifndef CALL_INTERFACE_PATCH_H
#define CALL_INTERFACE_PATCH_H

#include "codepatch.h"

class ASTClass;
class ASTFunction;

class CallInterfacePatch : public CodePatch
{
public:
   CallInterfacePatch(ASTClass& klass, const ASTFunction& function);

 // get/set
   ASTClass&          getClass();
   const ASTFunction& getFunction();

private:
   ASTClass&            mClass;
   const ASTFunction&   mFunction;
};

#endif // CALL_INTERFACE_PATCH_H
