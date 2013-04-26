
#ifndef STACK_IR_GENERATOR_H
#define STACK_IR_GENERATOR_H

#include "script/cil/cil.h"
#include "script/common/functionregistration.h"
#include "script/bytecode/irgenerator.h"
#include "script/bytecode/instruction.h"

#include "sbil.h"

class ASTFunction;
class ASTType;
class String;
class VirtualFunctionTableEntry;

class StackIRGenerator : public ByteCode::IRGenerator
{
public:
   StackIRGenerator();

   virtual char* generate(CompileContext& context, ByteCode::Program& program, const ASTFunction& function);

private:
   
   void generateInstructions(CompileContext& context, ByteCode::Program& program, const ASTFunction& function);
   void checkAndFixStack(const ByteCode::Program& program, const ASTFunction& function);
   void buildCode(ByteCode::Program& program, const ASTFunction& function);
};

#endif // STACK_IR_GENERATOR_H
