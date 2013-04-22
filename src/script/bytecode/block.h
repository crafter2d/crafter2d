
#ifndef CODEGEN_BLOCK_H
#define CODEGEN_BLOCK_H

#include <vector>

namespace CodeGen
{
   class Instruction;
   class Block;

   typedef std::vector<Block*> Blocks;

   class Block
   {
   public:
      int id;
      int start;

      Blocks to;
      Blocks from;
   };

   
};

#endif // CODEGEN_BLOCK_H
