
#include "irgenerator.h"

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "mod_yas/cil/cil.h"
#include "mod_yas/cil/function.h"
#include "mod_yas/cil/guard.h"
#include "mod_yas/cil/switchtabel.h"
#include "mod_yas/cil/switchtableentry.h"
#include "mod_yas/cil/switchtables.h"

#include "mod_yas/vm/virtualclass.h"
#include "mod_yas/vm/virtualcontext.h"
#include "mod_yas/vm/virtualguard.h"
#include "mod_yas/vm/virtualguards.h"
#include "mod_yas/vm/virtualfunction.h"
#include "mod_yas/vm/virtuallookuptable.h"

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

   bool IRGenerator::compile(VirtualContext& context, VirtualFunction& function)
   {
      const CIL::Instructions& instructions = function.getInstructions();
      if ( instructions.isEmpty() )
      {
         throw std::exception();
      }

      buildBlocks(function);

      if ( virGenerate(context, function) )
      {
         cleanup();

         return true;
      }

      return false;
   }

   bool IRGenerator::virGenerate(VirtualContext& context, VirtualFunction& entry)
   {
      PURE_VIRTUAL;
      return false;
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

   void IRGenerator::buildBlocks(VirtualFunction& entry)
   {
      const CIL::Instructions& instructions = entry.getInstructions();
      allocateInstructionBlocks(instructions.size());
      createBlock(0);

      buildGuards(entry);
      buildTables(entry);
      buildInstructions(instructions);
   }

   void IRGenerator::buildGuards(VirtualFunction& entry)
   {
      VirtualGuards& guards = entry.getGuards();
      for ( int index = 0; index < guards.size(); ++index )
      {
         VirtualGuard& guard = guards[index];
         buildGuardBlocks(guard);
      }
   }

   void IRGenerator::buildGuardBlocks(VirtualGuard& guard)
   {
      // For catch we create a new block. It is required for the stack code
      // generator, as it has to check whether the store local is an exception
      // handler or not.

      Block& block_start = createBlock(guard.locations[VirtualGuard::sStart]);
      block_start.pguard = &guard;
      block_start.guard_type = VirtualGuard::sStart;

      Block& block_catch = createBlock(guard.locations[VirtualGuard::sCatch]);
      block_catch.pguard = &guard;
      block_catch.guard_type = VirtualGuard::sCatch;

      Block& block_end   = createBlock(guard.locations[VirtualGuard::sEnd]);
      block_end.pguard = &guard;
      block_end.guard_type = VirtualGuard::sEnd;
      
      if ( guard.finalize )
      {
         Block& block_final = createBlock(guard.locations[VirtualGuard::sFinal]);
         block_final.pguard = &guard;
         block_final.guard_type = VirtualGuard::sFinal;
      }
   }

   void IRGenerator::buildTables(VirtualFunction& entry)
   {
      VirtualLookupTables& tables = entry.getLookupTables();
      for ( int index = 0; index < tables.size(); ++index )
      {
         VirtualLookupTable& table = tables[index];
         buildTableBlocks(table);
      }
   }

   void IRGenerator::buildTableBlocks(VirtualLookupTable& table)
   {
      Block& block_end = createBlock(table.getEnd());
      block_end.plookup = &table;
      block_end.lookup_type = Block::eEnd;

      if ( table.hasDefault() )
      {       
         Block& block_default = createBlock(table.getDefault());
         block_default.plookup = &table;
         block_default.lookup_type = Block::eDefault;
      }

      std::vector<int> values = table.getPositions();
      for ( std::size_t index = 0; index < values.size(); ++index )
      {
         int pos = values[index];
         const VirtualValue* pvalue = table.lookup(pos);
         ASSERT_PTR(pvalue);

         Block& block_case = createBlock(pos);
         block_case.plookup = &table;
         block_case.lookup_type = Block::eValue;
         block_case.lookup_value = *pvalue;
      }
   }

   void IRGenerator::buildInstructions(const CIL::Instructions& instructions)
   {
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
