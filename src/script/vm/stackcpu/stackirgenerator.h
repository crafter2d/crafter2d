
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

class String;
class VirtualFunctionTableEntry;

class StackIRGenerator : public CodeGen::IRGenerator
{
public:
   StackIRGenerator();

protected:
   virtual VirtualFunctionTableEntry* generate(VirtualContext& context, const CIL::Class& cilclass, const CIL::Function& cilfunction);

private:
   SBIL::Type typeToSBIL(const CIL::Type& type);
   SBIL::Type typeToSBIL(FunctionRegistration::Type type);

   CIL::Function* resolveFunction(VirtualContext& context, const String& name);
};

#endif // STACK_IR_GENERATOR_H
