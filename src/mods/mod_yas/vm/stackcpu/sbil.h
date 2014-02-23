
#ifndef STACK_BASED_CPU_H
#define STACK_BASED_CPU_H

namespace SBIL
{
   #define OPCODE(a,b,c) a,

   enum Opcode {
      #include "sbil.def"

      SBIL_last,
   };

   #undef OPCODE
}

#endif // STACK_BASED_CPU_H
