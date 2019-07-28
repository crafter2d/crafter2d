
#ifndef COMPILED_INSTRUCTION_H
#define COMPILED_INSTRUCTION_H

#include "cil.h"

struct CompiledInstruction
{
   CIL::Opcode opcode;

   union Value
   {
      int mInt;
      float mReal;
      void* mPtr;
   };
};

#endif // COMPILED_INSTRUCTION_H
