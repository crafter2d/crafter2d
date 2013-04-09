
#include "functionbuilder.h"

#include "core/string/string.h"
#include "core/defines.h"

#include "script/cil/function.h"

FunctionBuilder::FunctionBuilder():
   mInstructions(),
   mLabel(0)
{
}

// - Label operations

int FunctionBuilder::allocateLabel()
{
   return mLabel++;
}

void FunctionBuilder::addLabel(int id)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_label;
   inst.mInt   = id;
   mInstructions.push_back(inst);
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
}

void FunctionBuilder::end(CIL::Function& function)
{
   replaceLabels();
   removeNops();

   function.setInstructions(mInstructions);

   mInstructions.clear();
}

void FunctionBuilder::replaceLabels()
{
   if ( !mInstructions.empty() )
   {
      int firstline = 0;

      for ( std::size_t index = 0; index < mInstructions.size(); index++ )
      {
         CIL::Instruction& inst = mInstructions[index];

         if ( inst.opcode == CIL::CIL_jump
           || inst.opcode == CIL::CIL_jump_true
           || inst.opcode == CIL::CIL_jump_false
           || inst.opcode == CIL::CIL_enter_guard
           || inst.opcode == CIL::CIL_enter_guard_f )
         {
            int labelindex = findLabel(inst.mInt);
            ASSERT(labelindex != mInstructions.size());

            CIL::Instruction& labelinst = mInstructions[labelindex];
            labelinst.opcode = CIL::CIL_nop;
            inst.mInt = labelindex;
         }
      }
   }
}

void FunctionBuilder::removeNops()
{
   for ( int index = mInstructions.size() - 1; index >= 0; )
   {
      CIL::Instruction& inst = mInstructions[index];
      if ( inst.opcode == CIL::CIL_nop )
         mInstructions.erase(mInstructions.begin() + index);
      else
         index--;
   }
}

int FunctionBuilder::findLabel(int label) const
{
   std::size_t index = 0;
   for ( ; index < mInstructions.size(); index++ )
   {
      const CIL::Instruction& inst = mInstructions[index];
      if ( inst.opcode == CIL::CIL_label  )
      {
         break;
      }
   }

   return index;
}
