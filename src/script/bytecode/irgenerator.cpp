
#include "irgenerator.h"

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "script/cil/cil.h"
#include "script/cil/guard.h"

#include "script/ast/astfunction.h"
#include "script/compiler/compilecontext.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualfunctiontable.h"

#include "block.h"
#include "codepatch.h"
#include "exceptionguard.h"
#include "program.h"

namespace ByteCode
{

   IRGenerator::IRGenerator():
      mBlocks(),
      mPatches()
   {
   }

   IRGenerator::~IRGenerator()
   {
      cleanup();
   }

   // - Operations

   int IRGenerator::generate(CompileContext& context, const ASTFunction& function)
   {
      int result = -1;

      const CIL::Instructions& instructions = function.getInstructions();
      if ( instructions.empty() )
      {
         throw std::exception();
      }
      
      const CIL::Guards& guards = function.getGuards();
      buildBlocks(context, instructions, guards);

      result = virGenerate(context, function);
      cleanup();

      return result;
   }

   int IRGenerator::virGenerate(CompileContext& context, const ASTFunction& function)
   {
      PURE_VIRTUAL;
      return 0;
   };

   void IRGenerator::cleanup()
   {
      for ( std::size_t index = 0; index < mBlocks.size(); ++index )
      {
         delete mBlocks[index];
      }
      mBlocks.clear();

      for ( std::size_t index = 0; index < mPatches.size(); ++index )
      {
         delete mPatches[index];
      }
      mPatches.clear();
   }

   // - Block operations

   void IRGenerator::buildBlocks(CompileContext& context, const CIL::Instructions& instructions, const CIL::Guards& guards)
   {
      allocateInstructionBlocks(instructions.size());
      createBlock(0);

      for ( int index = 0; index < guards.size(); ++index )
      {
         const CIL::Guard& cilguard = guards[index];
         buildGuardBlocks(context, cilguard);
      }
      
      for ( std::size_t index = 0; index < instructions.size(); ++index )
      {
         const CIL::Instruction& inst = instructions[index];
         switch ( inst.opcode )
         {
            case CIL::CIL_jump:
            case CIL::CIL_jump_true:
            case CIL::CIL_jump_false:
               {
                  int target = index + inst.mInt;

                  Block& to = createBlock(target);
                  Block& from = createBlock(index);

                  to.from.push_back(&from);
                  from.to.push_back(&to);
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

   Block& IRGenerator::createBlock(int target)
   {
      Block* ptarget = mBlocks[target];
      if ( ptarget == NULL )
      {
         ptarget = new Block();
         ptarget->id = mBlocks.size();
         ptarget->start = target;
         mBlocks[target] = ptarget;
      }
      return *ptarget;
   }

   bool IRGenerator::hasBlock(int index) const
   {
      return mBlocks[index] != NULL;
   }
   
   Block* IRGenerator::getBlock(int index)
   {
      return mBlocks[index];
   }

   Blocks& IRGenerator::getBlocks()
   {
      return mBlocks;
   }

   void IRGenerator::buildGuardBlocks(CompileContext& context, const CIL::Guard& cilguard)
   {
      ExceptionGuard* pguard = new ExceptionGuard();
      context.getProgram().addGuard(pguard);

      // For catch we create a new block. It is required for the stack code
      // generator, as it has to check whether the store local is an exception
      // handler or not.

      Block& block_start = createBlock(cilguard.labels[CIL::Guard::sStart]);
      Block& block_catch = createBlock(cilguard.labels[CIL::Guard::sCatch]);
      Block& block_end   = createBlock(cilguard.labels[CIL::Guard::sEnd]);

      block_start.pguard = pguard;
      block_start.guard_type = ExceptionGuard::sStart;

      block_catch.pguard = pguard;
      block_catch.guard_type = ExceptionGuard::sCatch;

      block_end.pguard = pguard;
      block_end.guard_type = ExceptionGuard::sEnd;

      if ( pguard->finalize )
      {
         // If there is a final block, we link the start to the 

         Block& block_final = createBlock(cilguard.labels[CIL::Guard::sFinal]);

         block_final.pguard = pguard;
         block_final.guard_type = ExceptionGuard::sFinal;
      }
   }

   // - Patch operations

   void IRGenerator::addPatch(Patch* ppatch)
   {
      mPatches.push_back(ppatch);
   }

   void IRGenerator::applyPatches(char* pcode)
   {
      for ( std::size_t index = 0; index < mPatches.size(); ++index )
      {
         Patch* ppatch = mPatches[index];
         ppatch->apply(pcode);
      }
   }

} // namespace CodeGen