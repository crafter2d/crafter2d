
#ifndef BYTECODE_BLOCK_H
#define BYTECODE_BLOCK_H

#include <vector>

class VirtualGuard;

namespace ByteCode
{
   class Block;
   class Instruction;

   typedef std::vector<Block*> Blocks;

   class Block
   {
   public:
      int id;
      int start;
      int codepos;

      Instruction* pstart;
      Instruction* pcurrent;

      VirtualGuard* pguard;
      int           guard_type;
      
      Blocks to;
      Blocks from;

      Block();

      void addInstruction(Instruction* pinst);
   };
}

#endif // BYTECODE_BLOCK_H
