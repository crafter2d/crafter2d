
#include "functionbuilder.h"

#include "core/string/string.h"
#include "core/defines.h"

#include "yasc/cil/guard.h"
#include "yasc/cil/switchtabel.h"
#include "yasc/cil/switchtableentry.h"

FunctionBuilder::FunctionBuilder():
   mInstructions(),
   mGuards(),
   mSwitches(),
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

const CIL::SwitchTables& FunctionBuilder::getSwitchTables() const
{
   return mSwitches;
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
}

void FunctionBuilder::addGuard(CIL::Guard* pguard)
{
   mGuards.add(pguard);
}

int FunctionBuilder::addTable(CIL::SwitchTable* ptable)
{
   return mSwitches.add(ptable);
}

// - Emitters

void FunctionBuilder::emit(CIL::Opcode opcode)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   mInstructions.add(inst);
}

void FunctionBuilder::emit(CIL::Opcode opcode, int arg)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   inst.mInt   = arg;
   mInstructions.add(inst);
}

void FunctionBuilder::emit(CIL::Opcode opcode, double arg)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   inst.mReal  = arg;
   mInstructions.add(inst);
}

void FunctionBuilder::emit(CIL::Opcode opcode, const String& arg)
{
   CIL::Instruction inst;
   inst.opcode = opcode;
   inst.mString = new String(arg);
   mInstructions.add(inst);
}

// Building

void FunctionBuilder::start()
{
   mInstructions.clear();
   mLabels.clear();
   mGuards.clear();
   mSwitches.clear();
   mLabel = 0;
}

void FunctionBuilder::end()
{
   createJumps();
   createGuards();
   createSwitches();
}

void FunctionBuilder::createJumps()
{
   if ( !mLabels.empty() )
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

void FunctionBuilder::createGuards()
{
   for ( int index = 0; index < mGuards.size(); ++index )
   {
      CIL::Guard& guard = mGuards[index];

      for ( int label = 0; label < 4; ++label )
      {
         int target = mLabels[guard.labels[label]];
         guard.labels[label] = target;
      }
   }
}

void FunctionBuilder::createSwitches()
{
   int target;

   for ( int index = 0; index < mSwitches.size(); ++index )
   {
      CIL::SwitchTable& table = mSwitches[index];

      if ( table.hasDefault() )
      {
         target = mLabels[table.getDefault()];
         table.addDefault(target);
      }

      target = mLabels[table.getEnd()];
      table.addEnd(target);

      for ( int te = 0; te < table.size(); ++te )
      {
         CIL::SwitchTableEntry& entry = table[te];

         int target = mLabels[entry.label];
         entry.label = target;
      }
   }
}
