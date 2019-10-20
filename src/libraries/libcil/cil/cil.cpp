
#include "cil.h"

#include "core/string/string.h"

namespace CIL
{
#define OPCODE(a, b) { a, b },
   const OpcodeInfo opcodeinfo[CIL_last + 1] = {
      #include "cil.def"
   };
   #undef OPCODE

   // - Instruction class

   Instruction::Instruction():
      opcode(CIL_last)
   {
   }
   
   Instruction::Instruction(const Instruction& that):
      opcode(CIL_last)
   {
      assign(that);
   }

   Instruction::~Instruction()
   {
      switch ( opcodeinfo[opcode].type )
      {
      case T_STR:
         delete mString;
         break;
      default:
         break;
      }
   }

   const Instruction& Instruction::operator=(const Instruction& that)
   {
      assign(that);
      return *this;
   }

   void Instruction::assign(const Instruction& that)
   {
      opcode = that.opcode;
      switch ( opcodeinfo[opcode].type )
      {
      case T_INT:
         mInt = that.mInt;
         break;
      case T_REAL:
         mReal = that.mReal;
         break;
      case T_STR:
         mString = new String(*that.mString);
         break;
      case T_NONE:
      default:
         break;
      }
   }
}
