
#include "block.h"

#include "instruction.h"

namespace ByteCode
{
   Block::Block():
      id(0),
      start(0),
      pstart(NULL),
      pcurrent(NULL),
      to(),
      from()
   {
   }

   void Block::addInstruction(Instruction* pinst)
   {
      if ( pstart == NULL )
      {
         pstart = pinst;
         pcurrent = pinst;
      }
      else
      {
         pcurrent->next = pinst;
         pcurrent = pinst;
      }
   }
}
