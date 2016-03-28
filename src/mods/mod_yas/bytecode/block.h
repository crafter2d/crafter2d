
#ifndef BYTECODE_BLOCK_H
#define BYTECODE_BLOCK_H

#include <vector>

#include "mod_yas/vm/virtualvalue.h"

class VirtualGuard;
class VirtualLookupTable;

namespace ByteCode
{
   class Block;
   class Instruction;

   typedef std::vector<Block*> Blocks;

   class Block
   {
   public:
      enum LookupType { eDefault, eEnd, eValue };

      int      id;
      uint32_t start;
      uint32_t codepos;

      Instruction* pstart;
      Instruction* pcurrent;

      VirtualGuard* pguard;
      int           guard_type;

      VirtualLookupTable* plookup;
      VirtualValue        lookup_value;
      LookupType          lookup_type;
      
      Blocks to;
      Blocks from;

      Block();

      void addInstruction(Instruction* pinst);
   };
}

#endif // BYTECODE_BLOCK_H
