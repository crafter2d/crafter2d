
#ifndef CODEGEN_IR_CONTEXT_H
#define CODEGEN_IR_CONTEXT_H

#include "script/cil/classmap.h"
#include "script/common/classregistry.h"

#include "block.h"

namespace CodeGen
{
   class IRContext
   {
   public:
      IRContext();

      CIL::ClassMap classes;
      ClassRegistry* pclassregistry;
      
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
