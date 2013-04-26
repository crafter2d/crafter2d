
#include "jumppatch.h"

#include "block.h"
#include "instruction.h"

namespace ByteCode
{
   void JumpPatch::apply(char* pcode)
   {
      char* plocal = pcode + pos;

      // to block contains pointer to position in code
      int opcode = INST_OPCODE(*((int*)plocal));
      int offset = to->codepos - pos;      

      *((int*)plocal) = MAKE_INST(opcode,offset);
   }
}
