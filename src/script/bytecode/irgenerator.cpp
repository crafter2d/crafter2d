
#include "irgenerator.h"

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "script/ast/astfunction.h"
#include "script/cil/cil.h"
#include "script/cil/guard.h"
#include "script/compiler/compilecontext.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualguard.h"
#include "script/vm/virtualguards.h"
#include "script/vm/virtualfunctiontableentry.h"

#include "block.h"
#include "codepatch.h"
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

   VirtualFunctionTableEntry* IRGenerator::generate(CompileContext& context, const ASTFunction& function)
   {
      const CIL::Instructions& instructions = function.getInstructions();
      if ( instructions.empty() )
      {
         throw std::exception();
      }

      AutoPtr<VirtualFunctionTableEntry> result = new VirtualFunctionTableEntry();
      
      const CIL::Guards& guards = function.getGuards();
      buildBlocks(context, *result, instructions, guards);

      if ( virGenerate(context, *result, function) )
      {
         result->updateGuards();

         cleanup();
         return result.release();
      }

      return NULL;
   }

   bool IRGenerator::virGenerate(CompileContext& context, VirtualFunctionTableEntry& entry, const ASTFunction& function)
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

   void IRGenerator::buildBlocks(CompileContext& context, VirtualFunctionTableEntry& entry, const CIL::Instructions& instructions, const CIL::Guards& guards)
   {
      allocateInstructionBlocks(instructions.size());
      createBlock(0);

      for ( int index = 0; index < guards.size(); ++index )
      {
         const CIL::Guard& cilguard = guards[index];
         buildGuardBlocks(context, entry, cilguard);
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

   void IRGenerator::buildGuardBlocks(CompileContext& context, VirtualFunctionTableEntry& entry, const CIL::Guard& cilguard)
   {
      VirtualGuard* pguard = new VirtualGuard();
      entry.addGuard(pguard);

      // For catch we create a new block. It is required for the stack code
      // generator, as it has to check whether the store local is an exception
      // handler or not.

      Block& block_start = createBlock(cilguard.labels[CIL::Guard::sStart]);
      Block& block_catch = createBlock(cilguard.labels[CIL::Guard::sCatch]);
      Block& block_end   = createBlock(cilguard.labels[CIL::Guard::sEnd]);

      block_start.pguard = pguard;
      block_start.guard_type = VirtualGuard::sStart;

      block_catch.pguard = pguard;
      block_catch.guard_type = VirtualGuard::sCatch;

      block_end.pguard = pguard;
      block_end.guard_type = VirtualGuard::sEnd;

      if ( pguard->finalize )
      {
         // If there is a final block, we link the start to the 

         Block& block_final = createBlock(cilguard.labels[CIL::Guard::sFinal]);

         block_final.pguard = pguard;
         block_final.guard_type = VirtualGuard::sFinal;
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