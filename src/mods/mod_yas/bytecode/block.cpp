
#include "block.h"

#include "instruction.h"

namespace ByteCode
{
   Block::Block():
      id(0),
      start(0),
      codepos(0),
      pstart(nullptr),
      pcurrent(nullptr),
      pguard(nullptr),
      guard_type(-1),
      plookup(nullptr),
      lookup_value(),
      lookup_type(eDefault),
      to(),
      from()
   {
   }

   void Block::addInstruction(Instruction* pinst)
   {
      if ( pstart == nullptr )
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
