
#ifndef CIL_H
#define CIL_H

#include <vector>

class String;

#define T_NONE 0
#define T_INT  1
#define T_REAL 2
#define T_STR  3

namespace CIL
{
   #define OPCODE(a, b) a,

   enum Opcode {
      #include "cil.def"

      CIL_last,
   };

   #undef OPCODE

   struct OpcodeInfo
   {
      int opc;
      int type;
   };

   extern const OpcodeInfo opcodeinfo[CIL_last + 1];

   class Instruction
   {
   public:
      Instruction();
      Instruction(const Instruction& that);
      ~Instruction();

      const Instruction& operator=(const Instruction& that);

    // operations
      void assign(const Instruction& that);

    // data
      CIL::Opcode opcode;
      union
      {
         int mInt;
         double mReal;
         String* mString;
      };
   };
};

#endif // CIL_H
