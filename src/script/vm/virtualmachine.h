
#ifndef VM_H_
#define VM_H_

#include <deque>
#include <map>
#include <stack>

#include "core/defines.h"

#include "script/common/variant.h"
#include "script/compiler/compiler.h"

#include "virtualcompilecallback.h"
#include "virtualobjectreference.h"
#include "virtualfunctiontableentry.h"
#include "virtualcontext.h"

class VirtualInstruction;
class VirtualProgram;
class VirtualFunctionBase;
class VirtualFunctionTableEntry;
class VirtualException;

typedef std::deque<Variant> Stack;

class VirtualStackAccessor
{
public:
   VirtualStackAccessor(Stack& stack, int args): mStack(stack), mSize(args) {}

 // query
   VirtualObjectReference getThis() const
   {
      return getObject(mSize);
   }

   int getInt(int argument) const {
      Variant& value = getArgument(argument);
      if ( !value.isInt() )
         throw std::exception();
            
      return value.asInt();
   }

   std::string getString(int argument) const {
      Variant& value = getArgument(argument);
      if ( !value.isString() )
         throw std::exception();

      return value.asString();
   }

   VirtualObjectReference getObject(int argument) const {
      Variant& value = getArgument(argument);

      if ( !value.isObject() )
         throw std::exception();

      return value.asObject();
   }

 // return value
   bool hasResult() {
      return !mResult.isEmpty();
   }

   Variant& getResult() {
      return mResult;
   }

   void setResult(const VirtualObjectReference& object) {
      mResult = Variant(object);
   }

   void setResult(const std::string& value) {
      mResult = Variant(value);
   }

   void setResult(const Variant& value) {
      mResult = value;
   }

private:
   Variant& getArgument(int index) const {
      ASSERT(index <= mSize);
      return mStack[mStack.size() - index];
   }

   Stack&   mStack;
   Variant  mResult;
   int      mSize;
};

class VirtualMachine
{
public:
   VirtualMachine();

   typedef void (*callbackfnc)(VirtualMachine& machine, VirtualStackAccessor& accessor);

 // execution
   void execute(const std::string& classname, const std::string& function);
   void execute(const VirtualObjectReference& object, const std::string function);

 // object instantation
   VirtualObjectReference instantiate(const std::string& classname, int constructor = 2);
   VirtualObjectReference instantiateNative(const std::string& classname, void* pobject);
   VirtualArrayReference  instantiateArray();

private:
   friend class VirtualCompileCallback;

   class VirtualCall {
   public:

      class VirtualGuard
      {
      public:
         VirtualGuard(): mJumpTo(-1), mFinally(false) {}
         VirtualGuard(int jumpto, bool finally): mJumpTo(jumpto), mFinally(finally) {}

         int  mJumpTo;
         bool mFinally;
      };

      typedef std::deque<VirtualGuard> Guards;

      VirtualCall(): mInstructionPointer(0), mStackBase(0)
      {
      }

      const VirtualCall& operator=(const VirtualCall& that) {
         mInstructionPointer = that.mInstructionPointer;
         mStackBase = that.mStackBase;
         mGuards = that.mGuards;
         return *this;
      }

      void start(const VirtualFunctionTableEntry& entry, int stack) {
         mGuards.clear();
         mInstructionPointer = entry.mInstruction;
         mStackBase          = stack - entry.mArguments;
      }

      void jump(int address) {
         mInstructionPointer = address;
      }

      Guards mGuards;      
      int    mInstructionPointer;
      int    mStackBase;
   };
   
   typedef std::stack<VirtualCall> CallStack;
   typedef std::map<std::string, callbackfnc> Natives;

   enum State { eInit, eRunning, eFinalizing, eReturn };
   
 // execution
   void execute(const VirtualClass& vclass, const VirtualFunctionTableEntry& entry);
   void execute(const VirtualInstruction& instruction);

   Variant pop();
   
 // exception
   void throwException(const std::string& exceptionname);
   bool handleException(const VirtualException& e);

 // class loading
   VirtualClass* loadClass(const std::string& name);
   void classLoaded(VirtualClass* pclass);
   void createClass(const VirtualClass& aclass);

   VirtualContext         mContext;
   VirtualCompileCallback mCallback;
   Compiler               mCompiler;
   Stack                  mStack;
   CallStack              mCallStack;
   VirtualCall            mCall;
   VirtualObjectReference mException;
   Natives                mNatives;
   State                  mState;
   bool                   mRetVal;
   bool                   mLoaded;
};

#endif // VM_H_
