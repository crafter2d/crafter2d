
#ifndef REGISTER_IR_GENERATOR_H
#define REGISTER_IR_GENERATOR_H

namespace CIL
{
   class Class;
   class Function;
}

class RegisterIRGenerator
{
public:

   virtual bool generate(const CIL::Class& cilclass);

private:

   void generate(const CIL::Function& cilfunction);
};

#endif // REGISTER_IR_GENERATOR_H
