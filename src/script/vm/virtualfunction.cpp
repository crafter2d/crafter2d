
#include "virtualfunction.h"

#include "core/defines.h"

VirtualFunction::VirtualFunction():
   mInstructions()
{
}

void VirtualFunction::addInstruction(const VirtualInstruction& instruction)
{
   mInstructions.add(instruction);
}

void VirtualFunction::prepare()
{
   replaceLabels();
   removeNops();
}

void VirtualFunction::replaceLabels()
{
   if ( !mInstructions.isEmpty() )
   {
      int firstline = 0;

      for ( std::size_t index = 0; index < mInstructions.size(); index++ )
      {
         VirtualInstruction& inst = mInstructions[index];

         VirtualInstruction::Opcode opcode = inst.getOpcode();
         if ( opcode == VirtualInstruction::eJump
           || opcode == VirtualInstruction::eJumpTrue
           || opcode == VirtualInstruction::eJumpFalse
           || opcode == VirtualInstruction::eEnterGuard
           || opcode == VirtualInstruction::eEnterGuardF )
         {
            int labelindex = findLabel(inst.getArgument());
            ASSERT(labelindex != mInstructions.size());

            VirtualInstruction& labelinst = mInstructions[labelindex];
            labelinst.setOpcode(VirtualInstruction::eNop);
            inst.setArgument(labelindex);
         }
      }
   }
}

void VirtualFunction::removeNops()
{
   for ( int index = mInstructions.size() - 1; index >= 0; )
   {
      VirtualInstruction& inst = mInstructions[index];
      if ( inst.isNop() )
         mInstructions.remove(index);
      else
         index--;
   }
}

int VirtualFunction::findLabel(int label) const
{
   std::size_t index = 0;
   for ( ; index < mInstructions.size(); index++ )
   {
      const VirtualInstruction& inst = mInstructions[index];
      if ( inst.isLabel(label)  )
      {
         break;
      }
   }

   return index;
}
