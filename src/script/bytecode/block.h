
#ifndef BYTECODE_BLOCK_H
#define BYTECODE_BLOCK_H

#include <vector>

namespace ByteCode
{
   class Block;
   class ExceptionGuard;
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

      ExceptionGuard* pguard;
      int             guard_type;
      
      Blocks to;
      Blocks from;

      Block();

      void addInstruction(Instruction* pinst);
   };
}

#endif // BYTECODE_BLOCK_H
