
#ifndef FUNCTION_BUILDER_H
#define FUNCTION_BUILDER_H

#include <vector>

#include "cil.h"

class CompiledFunction;
class String;

class FunctionBuilder
{
public:
   FunctionBuilder();

 // storage
   void addLocal(CIL::Type* ptype);

 // label operations
   int  allocateLabel();
   void addLabel(int id);

 // emitting
   void emit(CIL::Opcode opcode);
   void emit(CIL::Opcode opcode, int arg);
   void emit(CIL::Opcode opcode, double arg);
   void emit(CIL::Opcode opcode, const String& arg);

 // building
   void reset();
   CompiledFunction* Build();

private:

 // operations
   void replaceLabels();
   void removeNops();
   int  findLabel(int label) const;

   CIL::Instructions mInstructions;
   CIL::Types        mLocals;
   int               mLabel;
};

#endif // FUNCTION_BUILDER_H
