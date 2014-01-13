
#include "jumppatch.h"

#include <math.h>

#include "block.h"
#include "instruction.h"

namespace ByteCode
{
   void JumpPatch::apply(char* pcode)
   {
      char* plocal = pcode + pos;

      // to block contains pointer to position in code
      // offset -1 instruction, as the jump instruction has been read already
      int opcode = INST_OPCODE(*((int*)plocal));
      int offset = to->codepos - pos - sizeof(int);
      
      int arg = (abs(offset) << 1);
      if ( offset < 0 )
         arg |= 1;

      *((int*)plocal) = MAKE_INST(opcode,arg);
   }
}
