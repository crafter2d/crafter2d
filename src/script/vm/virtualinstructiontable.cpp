
#include "virtualinstructiontable.h"

#include <sstream>

#include "script/common/variant.h"
#include "script/common/literaltable.h"
#include "script/common/literal.h"

VirtualInstructionTable::VirtualInstructionTable():
   mInstructions()
{
}

const VirtualInstruction& VirtualInstructionTable::operator[](int index) const
{
   return mInstructions[index];
}

VirtualInstruction& VirtualInstructionTable::operator[](int index)
{
   return mInstructions[index];
}

void VirtualInstructionTable::add(VirtualInstruction::Instruction instruction, int argument)
{
   VirtualInstruction inst(instruction, argument);
   add(inst);
}

void VirtualInstructionTable::add(const VirtualInstruction& instruction)
{
   mInstructions.push_back(instruction);
}

void VirtualInstructionTable::add(const VirtualInstructionTable& instructions)
{
   int offset = size();

   for ( int index = 0; index < instructions.size(); index++ )
   {
      const VirtualInstruction& instruction = instructions[index];

      switch ( instruction.getInstruction() )
      {
         case VirtualInstruction::eJump:
         case VirtualInstruction::eJumpTrue:
         case VirtualInstruction::eJumpFalse:
         case VirtualInstruction::eEnterGuard:
         case VirtualInstruction::eEnterGuardF:
            add(instruction.getInstruction(), instruction.getArgument() + offset);
            break;

         default:
            add(instruction);
            break;
      }
   }
}

int VirtualInstructionTable::size() const
{
   return mInstructions.size();
}

std::string VirtualInstructionTable::toString(const LiteralTable& table) const
{
   std::string result;
   std::ostringstream cout(std::ostringstream::out);
   
   int line = 0;

   for ( std::size_t index = 0; index < mInstructions.size(); index++, line++ )
   {
      const VirtualInstruction& inst = mInstructions[index];

      std::string text = VirtualInstruction::instructionToString(inst.getInstruction());

      cout.width(3);
      cout << line << ": " << text << " ";

      switch ( inst.getInstruction() )
      {
         case VirtualInstruction::eReserve:
         case VirtualInstruction::eCall:
         case VirtualInstruction::ePush: 
         case VirtualInstruction::eJump:
         case VirtualInstruction::eJumpTrue:
         case VirtualInstruction::eJumpFalse:
         case VirtualInstruction::eStore:
         case VirtualInstruction::eLoad:
         case VirtualInstruction::eStoreLocal:
         case VirtualInstruction::eLoadLocal:
         case VirtualInstruction::eStoreStatic:
         case VirtualInstruction::eLoadStatic:
         case VirtualInstruction::ePop:
         case VirtualInstruction::eRet:
            cout << inst.getArgument();
            break;

         case VirtualInstruction::eLoadLiteral:
            cout << inst.getArgument(); // table[inst.getArgument()].getValue().toString();
            break;

         default:
            break;
      }

      endl(cout);
   }

   return result;
}
