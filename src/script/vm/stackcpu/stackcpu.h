
#ifndef STACK_CPU_H
#define STACK_CPU_H

#include <deque>

#include "script/vm/cpu.h"
#include "script/vm/virtualstack.h"
#include "script/vm/virtualstackframe.h"

class VirtualObject;

class SCRIPT_API StackCPU : public CPU
{
   typedef std::deque<VM::StackFrame> CallStack;

public:
   StackCPU(VirtualMachine& vm);

 // query
   virtual ByteCode::IRGenerator* createIRGenerator();

 // execution
   virtual Variant execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry, int argc, Variant* pargs);
   virtual Variant execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry);
   virtual void    execute(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry);

 // garbage collection
   virtual void mark();
   
protected:

 // exception handling
   virtual String buildCallStack() const;
           bool   handleException(VirtualObject& exception);

private:
   enum State { eInit, eRunning, eFinalizing, eReturn, eDestruct };

 // execution
   void execute(VirtualContext& context);

   void call(VirtualContext& context, int symbolindex);
   void call(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry);

 // members
   CallStack      mCalls;
   VirtualStack   mStack;

   int            mIP;
   int            mFP;

   int            mSavedFP;
   State          mState;
};

#endif // STACK_CPU_H
