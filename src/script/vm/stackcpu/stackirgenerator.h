
#ifndef STACK_IR_GENERATOR_H
#define STACK_IR_GENERATOR_H

#include "script/cil/cil.h"
#include "script/common/functionregistration.h"
#include "script/vm/codegen/irgenerator.h"

#include "sbil.h"

class ASTFunction;
class ASTType;
class String;
class VirtualFunctionTableEntry;

class StackIRGenerator : public CodeGen::IRGenerator
{
public:
   StackIRGenerator();

   virtual char* generate(CompileContext& context, const ASTFunction& function);

private:
   SBIL::Type typeToSBIL(const ASTType& type);
   SBIL::Type typeToSBIL(FunctionRegistration::Type type);

   ASTFunction* resolveFunction(CompileContext& context, const String& name);
};

#endif // STACK_IR_GENERATOR_H
