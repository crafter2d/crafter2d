
#ifndef BYTECODE_BLOCK_H
#define BYTECODE_BLOCK_H

#include <vector>

namespace ByteCode
{
   class Instruction;
   class Block;

   typedef std::vector<Block*> Blocks;

   class Block
   {
   public:
      int id;
      int start;

      Instruction* pstart;
      Instruction* pcurrent;

      int codepos;

      Blocks to;
      Blocks from;

      Block();

      void addInstruction(Instruction* pinst);
   };
}

#endif // BYTECODE_BLOCK_H
