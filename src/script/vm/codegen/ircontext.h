
#ifndef CODEGEN_IR_CONTEXT_H
#define CODEGEN_IR_CONTEXT_H

#include "block.h"

namespace CodeGen
{
   class IRContext
   {
   public:
      IRContext();

   // operations
      void     allocateInstructionBlocks(int amount);
      Block*   createBlock(int target);
      void     insertBlock(int target);

   private:
    // operations
      void IRContext::linkBlocks(Block& from, Block& target);

      Blocks mBlocks;
   };
}

#endif // CODEGEN_IR_CONTEXT_H
