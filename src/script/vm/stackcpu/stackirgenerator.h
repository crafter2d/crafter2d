
#ifndef STACK_IR_GENERATOR_H
#define STACK_IR_GENERATOR_H

namespace CIL
{
   class Class;
   class Function;
}

class VirtualFunctionTableEntry;

class StackIRGenerator
{
public:

   bool generate(const CIL::Class& cilclass);

private:

   bool generate(const CIL::Function& cilfunction);

   VirtualFunctionTableEntry* resolveFunction(const String& name);
};

#endif // STACK_IR_GENERATOR_H
