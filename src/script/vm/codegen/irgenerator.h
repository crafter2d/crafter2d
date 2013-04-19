
#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "script/cil/cil.h"

#include "block.h"

class ASTFunction;
class CompileContext;
class VirtualContext;
class VirtualClass;
class VirtualFunctionTableEntry;

namespace CodeGen
{
   class IRGenerator
   {
   public:
      IRGenerator();

      virtual char* generate(CompileContext& context, const ASTFunction& function) = 0;

   protected:
    // block operations
      void     buildBlocks(CompileContext& context, const CIL::Instructions& instructions);
      Block*   getBlock(int target);

   private:

    // block operations
      void     allocateInstructionBlocks(int amount);
      Block*   createBlock(int target);
      void     insertBlock(int target);

      Blocks mBlocks;
   };
}

#endif // IR_GENERATOR_H
