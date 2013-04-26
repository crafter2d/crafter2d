
#ifndef REGISTER_IR_GENERATOR_H
#define REGISTER_IR_GENERATOR_H

#include "script/bytecode/irgenerator.h"

class RegisterIRGenerator : public ByteCode::IRGenerator
{
public:

   virtual char* RegisterIRGenerator::generate(CompileContext& context, const ASTFunction& function);

private:
};

#endif // REGISTER_IR_GENERATOR_H
