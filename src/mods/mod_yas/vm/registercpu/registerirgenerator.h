
#ifndef REGISTER_IR_GENERATOR_H
#define REGISTER_IR_GENERATOR_H

#include "mod_yas/bytecode/irgenerator.h"

class RegisterIRGenerator : public ByteCode::IRGenerator
{
public:

protected:
   virtual bool virGenerate(VirtualContext& context, VirtualFunction& entry) override;

private:
};

#endif // REGISTER_IR_GENERATOR_H
