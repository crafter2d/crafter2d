
#ifndef FUNCTION_BUILDER_H
#define FUNCTION_BUILDER_H

#include <vector>

#include "cil.h"

class String;

class FunctionBuilder
{
   typedef std::vector<CIL::Instruction> Instructions;

public:
   FunctionBuilder();

 // label operations
   int  allocateLabel();
   void addLabel(int id);

 // builder operations
   void addPush(int arg);
   void addPushThis();
   void addPushNull();
   void addPop(int arg);
   void addDup();
   void addNew(const String& arg);
   void addNewArray(int dimension);
   void addConvInt();
   void addConvReal();
   void addConvString();
   void addCall(const String& name);
   void addCallSuper();
   void addCallStatic();
   void addCallNative(int index);
   void addRet(int arg);
   void addAdd();
   void addSub();
   void addMul();
   void addDiv();
   void addRem();
   void addOr();
   void addAnd();
   void addXor();
   void addNot();
   void addNeg();
   void addShiftLeft();
   void addShiftRight();
   void addLoad(int arg);
   void addStore(int arg);
   void addLoadArg(int arg);
   void addStoreArg(int arg);
   void addLoadLocal(int arg);
   void addStoreLocal(int arg);
   void addLoadElem(int arg);
   void addStoreElem(int arg);
   void addLoadStatic(int arg);
   void addStoreStatic(int arg);
   void addCmpEq();
   void addCmpNe();
   void addCmpGt();
   void addCmpGe();
   void addCmpLe();
   void addCmpLT();
   void addIsNull();
   void addJump(int label);
   void addJumpTrue(int label);
   void addJumpFalse(int label);
   void addConst(int value);
   void addConst(double value);
   void addConst(const String& value);
   void addLookup(const String& name);
   void addInstanceOf(const String& name);
   void addEnterGuard(int label);
   void addEnterGuardFinal(int label);
   void addLeaveGuard();
   void addThrow();

   void emit(CIL::Opcode opcode);
   void emit(CIL::Opcode opcode, int arg);
   void emit(CIL::Opcode opcode, double arg);
   void emit(CIL::Opcode opcode, const String& arg);

private:

   Instructions   mInstructions;
   int            mLabel;
};

#endif // FUNCTION_BUILDER_H
