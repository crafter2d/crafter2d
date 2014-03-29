
#include "block.h"

#include "instruction.h"

namespace ByteCode
{
   Block::Block():
      id(0),
      start(0),
      codepos(0),
      pstart(NULL),
      pcurrent(NULL),
      pguard(NULL),
      guard_type(-1),
      plookup(NULL),
      lookup_value(),
      lookup_type(eDefault),
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
