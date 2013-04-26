
#include "irgenerator.h"

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "script/cil/cil.h"

#include "script/vm/virtualclass.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualfunctiontable.h"

#include "block.h"
#include "codepatch.h"

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

   char* IRGenerator::generate(CompileContext& context, Program& program, const ASTFunction& function)
   {
      PURE_VIRTUAL;
      return NULL;
   }

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

   void IRGenerator::buildBlocks(CompileContext& context, const CIL::Instructions& instructions)
   {
      allocateInstructionBlocks(instructions.size());
      createBlock(0);

      for ( unsigned index = 0; index < instructions.size(); ++index )
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
      Block* ptarget = new Block();
      ptarget->id = mBlocks.size();
      ptarget->start = target;
      mBlocks[target] = ptarget;
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