
#include "functionbuilder.h"

#include "core/string/string.h"
#include "core/defines.h"

FunctionBuilder::FunctionBuilder():
   mInstructions(),
   mGuards(),
   mLabels(),
   mLabel(0)
{
}

// - Get/set

const CIL::Instructions& FunctionBuilder::getInstructions() const
{
   return mInstructions;
}

const CIL::Guards& FunctionBuilder::getGuards() const
{
   return mGuards;
}

// - Label operations

int FunctionBuilder::allocateLabel()
{
   return mLabel++;
}

void FunctionBuilder::addLabel(int id)
{
   mLabels.resize(mLabel);
   mLabels[id] = mInstructions.size();
   /*
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_label;
   inst.mInt   = id;
   mInstructions.push_back(inst);
   */
}

void FunctionBuilder::addGuard(CIL::Guard* pguard)
{
   mGuards.add(pguard);
}

// - Emitters

void FunctionBuilder::emit(CIL::Opcode opcode)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   mInstructions.push_back(inst);
}

void FunctionBuilder::emit(CIL::Opcode opcode, int arg)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::emit(CIL::Opcode opcode, double arg)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   inst.mReal  = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::emit(CIL::Opcode opcode, const String& arg)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   inst.mString = new String(arg);
   mInstructions.push_back(inst);
}

void FunctionBuilder::emit(CIL::Opcode opcode, void* parg)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   inst.mPtr = parg;
   mInstructions.push_back(inst);
}

// Building

void FunctionBuilder::start()
{
   mInstructions.clear();
   mLabels.clear();
   mLabel = 0;
}

void FunctionBuilder::end()
{
   createJumps();
   removeNops();
}

void FunctionBuilder::createJumps()
{
   if ( mLabels.size() > 0 )
   {
      for ( std::size_t index = 0; index < mInstructions.size(); index++ )
      {
         CIL::Instruction& inst = mInstructions[index];

         switch ( inst.opcode )
         {
            case CIL::CIL_jump:
            case CIL::CIL_jump_true:
            case CIL::CIL_jump_false:
               {
                  int target = mLabels[inst.mInt];
                  inst.mInt = target - index;
               }
               break;
         }
      }
   }
}

void FunctionBuilder::removeNops()
{
   for ( int index = mInstructions.size() - 1; index >= 0; --index )
   {
      CIL::Instruction& inst = mInstructions[index];
      if ( inst.opcode == CIL::CIL_nop )
      {
         mInstructions.erase(mInstructions.begin() + index);
      }
   }
}
