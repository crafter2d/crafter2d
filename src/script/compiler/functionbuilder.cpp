
#include "functionbuilder.h"

#include "core/string/string.h"

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

// - Builder operations

void FunctionBuilder::addPush(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_push;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addPop(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_pop;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addLoadLocal(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldloc;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addStoreLocal(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_stloc;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addNew(const String& arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_new;
   inst.mString = new String(arg);
   mInstructions.push_back(inst);
}

void FunctionBuilder::addNewArray(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_newarray;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addLoadElem(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldelem;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addStoreElem(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_stelem;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addJump(int label)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_jump;
   inst.mInt   = label;
   mInstructions.push_back(inst);
}
   
void FunctionBuilder::addJumpTrue(int label)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_jump_true;
   inst.mInt   = label;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addJumpFalse(int label)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_jump_false;
   inst.mInt   = label;
   mInstructions.push_back(inst);
}
