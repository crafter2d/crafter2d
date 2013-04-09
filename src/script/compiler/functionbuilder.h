
#ifndef FUNCTION_BUILDER_H
#define FUNCTION_BUILDER_H

#include <vector>

#include "script/cil/cil.h"

namespace CIL
{
   class Function;
}

class ASTFunction;
class String;

class FunctionBuilder
{
public:
   FunctionBuilder();
   
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
   void start();
   void end(CIL::Function& function);

private:

 // operations
   void replaceLabels();
   void removeNops();
   int  findLabel(int label) const;

   CIL::Instructions mInstructions;
   int               mLabel;
};

#endif // FUNCTION_BUILDER_H
