
#include "irgenerator.h"

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "script/ast/astfunction.h"
#include "script/cil/cil.h"
#include "script/cil/guard.h"
#include "script/cil/switchtabel.h"
#include "script/cil/switchtableentry.h"
#include "script/cil/switchtables.h"
#include "script/compiler/compilecontext.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualguard.h"
#include "script/vm/virtualguards.h"
#include "script/vm/virtualfunctiontableentry.h"
#include "script/vm/virtuallookuptable.h"

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
      
      buildBlocks(context, *result, function);

      if ( virGenerate(context, *result, function) )
      {
         result->updateGuards();
         result->updateLookupTables();

         cleanup();
         return result.release();
      }

      return NULL;
   }

   bool IRGenerator::virGenerate(CompileContext& context, VirtualFunctionTableEntry& entry, const ASTFunction& function)
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

   void IRGenerator::buildBlocks(CompileContext& context, VirtualFunctionTableEntry& entry, const ASTFunction& function)
   {
      const CIL::Instructions& instructions = function.getInstructions();
      allocateInstructionBlocks(instructions.size());
      createBlock(0);

      buildGuards(entry, function.getGuards());      
      buildTables(entry, function.getSwitchTables());
      buildInstructions(instructions);
   }

   void IRGenerator::buildGuards(VirtualFunctionTableEntry& entry, const CIL::Guards& cilguards)
   {
      for ( int index = 0; index < cilguards.size(); ++index )
      {
         const CIL::Guard& cilguard = cilguards[index];
         buildGuardBlocks(entry, cilguard);
      }
   }

   void IRGenerator::buildGuardBlocks(VirtualFunctionTableEntry& entry, const CIL::Guard& cilguard)
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

   void IRGenerator::buildTables(VirtualFunctionTableEntry& entry, const CIL::SwitchTables& tables)
   {
      for ( int index = 0; index < tables.size(); ++index )
      {
         const CIL::SwitchTable& table = tables[index];
         buildTableBlocks(entry, table);
      }
   }

   void IRGenerator::buildTableBlocks(VirtualFunctionTableEntry& entry, const CIL::SwitchTable& table)
   {
      VirtualLookupTable* plookup = new VirtualLookupTable();
      entry.addLookupTable(plookup);

      plookup->setEnd(table.getEnd());
      Block& block_end = createBlock(table.getEnd());
      block_end.plookup = plookup;
      block_end.lookup_type = 2;

      if ( table.hasDefault() )
      {
         plookup->setDefault(table.getDefault());
         
         Block& block_default = createBlock(table.getDefault());
         block_default.plookup = plookup;
         block_default.lookup_type = 1;
      }

      for ( int index = 0; index < table.size(); ++index )
      {
         const CIL::SwitchTableEntry& entry = table[index];

         Block& block_case = createBlock(entry.label);
         block_case.plookup = plookup;
         block_case.lookup_value = entry.value;
         block_case.lookup_type = 3;
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
