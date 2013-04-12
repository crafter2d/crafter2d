
#include "ircontext.h" 

namespace CodeGen
{
   IRContext::IRContext():
      mBlocks()
   {
   }

   void IRContext::allocateInstructionBlocks(int amount)
   {
      mBlocks.resize(amount);
   }

   Block* IRContext::createBlock(int target)
   {
      Block* ptarget = new Block();
      ptarget->id = mBlocks.size();
      ptarget->start = target;
      return ptarget;
   }

   void IRContext::insertBlock(int target)
   {
      if ( mBlocks[target] == NULL )
      {
         Block* ptarget = createBlock(target);
         mBlocks[target] = ptarget;
      }
   }

   void IRContext::linkBlocks(Block& from, Block& target)
   {

   }
}