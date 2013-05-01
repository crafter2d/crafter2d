
#ifndef STACK_CPU_H
#define STACK_CPU_H

#include <stack>

#include "script/vm/cpu.h"
#include "script/vm/virtualstack.h"
#include "script/vm/virtualstackframe.h"

class VirtualObject;

class SCRIPT_API StackCPU : public CPU
{
   typedef std::stack<VM::StackFrame> CallStack;

public:
   StackCPU(VirtualMachine& vm);

   virtual ByteCode::IRGenerator* createIRGenerator();

   virtual void execute(VirtualContext& context, const VirtualClass& pclass, const VirtualFunctionTableEntry& entry);

private:
 // execution
   void call(VirtualContext& context, int symbolindex);
   void call(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry);

   VirtualObject* instantiate(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry);

   CallStack      mCalls;
   VirtualStack   mStack;

   int            mIP;
   int            mFP;
};

#endif // STACK_CPU_H
