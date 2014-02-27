
#ifndef STACK_IR_GENERATOR_H
#define STACK_IR_GENERATOR_H

#include "mod_yas/cil/cil.h"
#include "mod_yas/common/functionregistration.h"
#include "mod_yas/bytecode/irgenerator.h"
#include "mod_yas/bytecode/instruction.h"

#include "sbil.h"

class ASTType;
class String;
class VirtualContext;
class VirtualFunctionTableEntry;

class StackIRGenerator : public ByteCode::IRGenerator
{
public:
   StackIRGenerator();

protected:
   virtual bool virGenerate(VirtualContext& context, VirtualFunction& entry) override;

private:
   
   void generateInstructions(VirtualContext& context, const VirtualFunction& function);
   void checkAndFixStack(VirtualContext& context, const VirtualFunction& function);
   int  buildCode(VirtualContext& context, VirtualFunction& function);
};

#endif // STACK_IR_GENERATOR_H
