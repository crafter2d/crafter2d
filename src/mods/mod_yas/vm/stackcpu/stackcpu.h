
#ifndef STACK_CPU_H
#define STACK_CPU_H

#include <deque>

#include "mod_yas/bytecode/cpu.h"
#include "mod_yas/vm/virtualstack.h"
#include "mod_yas/vm/virtualstackframe.h"

class VirtualObject;
class VirtualGuard;

class StackCPU : public ByteCode::CPU
{
   typedef std::deque<VM::StackFrame> CallStack;

public:
   explicit StackCPU(VirtualMachine& vm);

 // query
   virtual ByteCode::IRGenerator* createIRGenerator();

 // execution
   virtual VirtualValue execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry, int argc, VirtualValue* pargs) override;
   virtual void         executeStatic(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry) override;

 // garbage collection
   virtual void mark() override;
   
protected:

 // execution
   virtual void execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry);

 // exception handling
   virtual String buildCallStack() const;
   virtual bool   handleException(VirtualContext& context, VirtualObject& exception);

private:
   enum State { eInit, eRunning, eExceptionHandling, eFinalizing, eReturn, eDestruct };

 // execution
   void execute(VirtualContext& context, const VirtualFunctionTableEntry& entry);

   void call(VirtualContext& context, int symbolindex);
   void call(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry);

   void compile(VirtualContext& context, const VirtualFunctionTableEntry& entry);

 // members
         CallStack      mCalls;
         VirtualStack   mStack;
   const VirtualGuard*  mpActiveGuard;

   const char*    mpCode;

   int            mIP;
   int            mFP;

   int            mSavedFP;
   State          mState;
};

#endif // STACK_CPU_H
