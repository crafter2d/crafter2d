
#include "irgenerator.h"

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "script/cil/cil.h"

#include "script/vm/virtualclass.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualfunctiontable.h"

#include "block.h"

namespace CodeGen
{

   IRGenerator::IRGenerator():
      mBlocks()
   {
   }

   // - Operations

   char* IRGenerator::generate(CompileContext& context, const ASTFunction& function)
   {
      PURE_VIRTUAL;
      return NULL;
   }

   // - Block operations

   void IRGenerator::buildBlocks(CompileContext& context, const CIL::Instructions& instructions)
   {
      allocateInstructionBlocks(instructions.size());
      insertBlock(0);

      for ( unsigned index = 0; index < instructions.size(); ++index )
      {
         const CIL::Instruction& inst = instructions[index];
         switch ( inst.opcode )
         {
            case CIL::CIL_jump:
            case CIL::CIL_jump_true:
            case CIL::CIL_jump_false:
               // insert new block for code between this and target
               {
                  int target = inst.mInt;
                  insertBlock(target);
               }
               break;

            default:
               // no jump, so nothing happens
               break;
         }
      }
   }

   void IRGenerator::allocateInstructionBlocks(int amount)
   {
      mBlocks.resize(amount);
   }

   Block* IRGenerator::createBlock(int target)
   {
      Block* ptarget = new Block();
      ptarget->id = mBlocks.size();
      ptarget->start = target;
      return ptarget;
   }

   void IRGenerator::insertBlock(int target)
   {
      if ( mBlocks[target] == NULL )
      {
         Block* ptarget = createBlock(target);
         mBlocks[target] = ptarget;
      }
   }

   Block* IRGenerator::getBlock(int target)
   {
      return mBlocks[target] != NULL ? mBlocks[target] : NULL;
   }

} // namespace CodeGen