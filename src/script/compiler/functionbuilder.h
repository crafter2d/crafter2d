
#ifndef FUNCTION_BUILDER_H
#define FUNCTION_BUILDER_H

#include <vector>

#include "cil.h"

namespace CIL
{
   class Function;
}

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
   void emit(CIL::Opcode opcode, void* parg);

 // building
   void reset();
   CIL::Function* build();

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
