
#ifndef STACK_IR_GENERATOR_H
#define STACK_IR_GENERATOR_H

#include "script/cil/cil.h"
#include "script/common/functionregistration.h"
#include "script/vm/codegen/irgenerator.h"

#include "sbil.h"

namespace CIL
{
   class Class;
   class Function;
}

namespace CodeGen
{
   struct IRCall;
}

class String;
class VirtualFunctionTableEntry;

class StackIRGenerator : public CodeGen::IRGenerator
{
public:
   StackIRGenerator();

protected:
   virtual bool generate(CodeGen::IRContext& context, const CIL::Class& cilclass, const CIL::Function& cilfunction);

private:
   SBIL::Type typeToSBIL(const CIL::Type& type);
   SBIL::Type typeToSBIL(FunctionRegistration::Type type);

   CIL::Function* resolveFunction(CodeGen::IRContext& context, const String& name);
};

#endif // STACK_IR_GENERATOR_H
