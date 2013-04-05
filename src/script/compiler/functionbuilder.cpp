
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

void FunctionBuilder::addLabel(int id)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_label;
   inst.mInt   = id;
   mInstructions.push_back(inst);
}

// - Builder operations

void FunctionBuilder::addPush(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_push;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addPushThis()
{
   // convenience method, loads the this argument (always first)
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldarg;
   inst.mInt   = 0;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addPushNull()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_pushnull;
   inst.mInt   = 0;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addPop(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_pop;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addDup()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_dup;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addNew(const String& arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_new;
   inst.mString = new String(arg);
   mInstructions.push_back(inst);
}

void FunctionBuilder::addNewArray(int dimension)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_new;
   inst.mInt = dimension;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addConvInt()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_conv_int;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addConvReal()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_conv_real;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addConvString()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_conv_string;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCall(const String& name)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_call;
   inst.mString = new String(name);
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCallSuper()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_call_super;
   inst.mString = new String();
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCallStatic()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_call_static;
   inst.mString = new String();
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCallNative(int index)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_call_native;
   inst.mInt = index;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addRet(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ret;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addAdd()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_add;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addSub()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_sub;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addMul()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_mul;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addDiv()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_div;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addRem()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_rem;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addOr()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_or;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addAnd()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_and;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addXor()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_xor;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addNot()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_not;
   mInstructions.push_back(inst);
}
   
void FunctionBuilder::addNeg()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_neg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addShiftLeft()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_shl;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addShiftRight()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_shr;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addLoad(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_load;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addStore(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldloc;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addLoadArg(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldarg;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}
   
void FunctionBuilder::addStoreArg(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_starg;
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

void FunctionBuilder::addLoadStatic(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldstatic;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}
   
void FunctionBuilder::addStoreStatic(int arg)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ststatic;
   inst.mInt   = arg;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCmpEq()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_cmpeq;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCmpNe()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_cmpne;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCmpGt()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_cmpgt;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCmpGe()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_cmpge;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCmpLe()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_cmple;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addCmpLT()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_cmplt;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addIsNull()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_isnull;
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

void FunctionBuilder::addConst(int value)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldcon;
   inst.mInt   = value;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addConst(double value)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldreal;
   inst.mReal   = value;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addConst(const String& value)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_ldstr;
   inst.mString = new String(value);
   mInstructions.push_back(inst);
}

void FunctionBuilder::addLookup(const String& name)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_lookup;
   inst.mString = new String(name);
   mInstructions.push_back(inst);
}

void FunctionBuilder::addInstanceOf(const String& name)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_instanceof;
   inst.mString = new String(name);
   mInstructions.push_back(inst);
}

void FunctionBuilder::addEnterGuard(int label)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_enter_guard;
   inst.mInt   = label;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addEnterGuardFinal(int label)
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_enter_guard_f;
   inst.mInt   = label;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addLeaveGuard()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_leave_guard;
   mInstructions.push_back(inst);
}

void FunctionBuilder::addThrow()
{
   CIL::Instruction inst;
   inst.opcode = CIL::CIL_throw;
   mInstructions.push_back(inst);
}

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
