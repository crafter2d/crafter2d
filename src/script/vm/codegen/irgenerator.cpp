
#include "irgenerator.h"

#include "core/defines.h"

#include "script/cil/cil.h"
#include "script/cil/class.h"

#include "block.h"
#include "ircontext.h"

namespace CodeGen
{

   IRGenerator::IRGenerator()
   {
   }

   // - Operations

   bool IRGenerator::generate(IRContext& context, const CIL::Class& cilclass)
   {
      const CIL::Class::Functions& functions = cilclass.getFunctions();
      for ( unsigned index = 0; index < functions.size(); ++index )
      {
         const CIL::Function& function = *functions[index];
         if ( !generate(context, cilclass, function) )
         {
            return false;
         }
      }

      return true;
   }

   bool IRGenerator::generate(IRContext& context, const CIL::Class& cilclass, const CIL::Function& cilfunction)
   {
      PURE_VIRTUAL;
      return false;
   }

   void IRGenerator::buildBlocks(IRContext& context, const CIL::Instructions& instructions)
   {
      context.allocateInstructionBlocks(instructions.size());
      context.insertBlock(0);

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
                  context.insertBlock(target);
               }
               break;

            default:
               // no jump, so nothing happens
               break;
         }
      }
   }

} // namespace CodeGen