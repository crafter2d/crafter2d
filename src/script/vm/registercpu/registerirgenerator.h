
#ifndef REGISTER_IR_GENERATOR_H
#define REGISTER_IR_GENERATOR_H

#include "script/vm/codegen/irgenerator.h"

class RegisterIRGenerator : public CodeGen::IRGenerator
{
public:

   virtual char* RegisterIRGenerator::generate(CompileContext& context, const ASTFunction& function);

private:
};

#endif // REGISTER_IR_GENERATOR_H
