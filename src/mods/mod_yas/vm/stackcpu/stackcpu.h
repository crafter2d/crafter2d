
#ifndef STACK_CPU_H
#define STACK_CPU_H

#include <deque>

#include "mod_yas/bytecode/cpu.h"
#include "mod_yas/vm/virtualstack.h"
#include "mod_yas/vm/virtualstackframe.h"

class VirtualObject;
class VirtualGuard;

class SCRIPT_API StackCPU : public ByteCode::CPU
{
   typedef std::deque<VM::StackFrame> CallStack;

public:
   StackCPU(VirtualMachine& vm);

 // query
   virtual ByteCode::IRGenerator* createIRGenerator();

 // execution
   virtual VirtualValue execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry, int argc, VirtualValue* pargs);
   virtual void         executeStatic(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry);

 // garbage collection
   virtual void mark();
   
protected:

 // exception handling
   virtual String buildCallStack() const;
   virtual bool   handleException(VirtualContext& context, VirtualObject& exception);

private:
   enum State { eInit, eRunning, eExceptionHandling, eFinalizing, eReturn, eDestruct };

 // execution
   void execute(VirtualContext& context);

   void call(VirtualContext& context, int symbolindex);
   void call(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry);

 // members
         CallStack      mCalls;
         VirtualStack   mStack;
   const VirtualGuard*  mpActiveGuard;

   int            mIP;
   int            mFP;

   int            mSavedFP;
   State          mState;
};

#endif // STACK_CPU_H
