/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef VM_H_
#define VM_H_

#include <deque>
#include <map>
#include <stack>

#include "script/script_base.h"

#include "core/defines.h"

#include "script/common/variant.h"
#include "script/compiler/compiler.h"
#include "script/gc/garbagecollector.h"

#include "virtualcompilecallback.h"
#include "virtualobjectreference.h"
#include "virtualfunctiontableentry.h"
#include "virtualmachineobjectobserver.h"
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
   VirtualStackAccessor(Stack& stack): mStack(stack), mSize(stack.back().asInt())
   {
   }

 // query
   VirtualObjectReference& getThis() const
   {
      return getObject(0);
   }

   int getInt(int argument) const {
      Variant& value = getArgument(argument);
      if ( !value.isInt() )
         throw std::exception();

      return value.asInt();
   }

   double getReal(int argument) const {
      Variant& value = getArgument(argument);
      if ( !value.isReal() )
         throw std::exception();

      return value.asReal();
   }

   const std::string& getString(int argument) const {
      Variant& value = getArgument(argument);
      if ( !value.isString() )
         throw std::exception();

      return value.asString();
   }

   bool getBoolean(int argument) const {
      Variant& value = getArgument(argument);
      if ( !value.isBool() )
         throw std::exception();

      return value.asBool();
   }

   VirtualObjectReference& getObject(int argument) const {
      Variant& value = getArgument(argument);

      if ( !value.isObject() )
         throw std::exception();

      return value.asObject();
   }

   VirtualArrayReference& getArray(int argument) const {
      Variant& value = getArgument(argument);

      if ( !value.isArray() )
         throw std::exception();

      return value.asArray();
   }

 // return value
   bool hasResult() {
      return !mResult.isEmpty();
   }

   Variant& getResult() {
      return mResult;
   }

   void setResult(const VirtualObjectReference& object) {
      if ( !object.isNull() )
      {
         mResult = Variant(object);
      }
   }

   void setResult(int value) {
      mResult = Variant(value);
   }

   void setResult(double value) {
      mResult = Variant(value);
   }

   void setResult(bool value) {
      mResult = Variant(value);
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
      return mStack[mStack.size() - (mSize + 1) + index];
      // 0 1 2 3 -> ssize = 4; size = 3
      // index 0 -> 4 - 3 = 1
   }

   Stack&   mStack;
   Variant  mResult;
   int      mSize;
};

class SCRIPT_API VirtualMachine
{
public:
   explicit VirtualMachine(VirtualContext& context);
   ~VirtualMachine();

   typedef void (*callbackfnc)(VirtualMachine& machine, VirtualStackAccessor& accessor);

 // initialization
   void initialize();

 // query
   const VirtualObjectReference& getNativeObject(void* pobject) const;

 // loading
   bool loadClass(const std::string& classname);

   void registerCallback(const std::string& name, callbackfnc callback);

 // stack access
   int popInt();
   double popReal();
   bool popBoolean();
   std::string popString();

   void push(int value);
   void push(double value);
   void push(bool value);
   void push(const std::string& value);
   void push(const VirtualObjectReference& object);

 // execution
   bool execute(const std::string& classname, const std::string& function);
   void execute(const VirtualObjectReference& object, const std::string& function);

 // exception handling
   std::string buildCallStack() const;

 // object instantation
   VirtualObjectReference instantiate(const std::string& classname, int constructor = 2, void* pobject = NULL);
   VirtualObjectReference instantiateNative(const std::string& classname, void* pobject, bool owned = true);
   VirtualObjectReference instantiateShare(const VirtualObjectReference& origin);
   VirtualArrayReference  instantiateArray();

 // observing
   VirtualObjectReference lookupNative(void* pobject);
   void registerNative(VirtualObjectReference& object, void* pnative);
   void unregisterNative(VirtualObjectReference& object);

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

      VirtualCall(): mpClass(NULL), mpEntry(NULL), mInstructionPointer(0), mStackBase(0)
      {
      }

      const VirtualCall& operator=(const VirtualCall& that) {
         mpClass = that.mpClass;
         mpEntry = that.mpEntry;
         mInstructionPointer = that.mInstructionPointer;
         mStackBase = that.mStackBase;
         mGuards = that.mGuards;
         return *this;
      }

      void start(const VirtualClass& vclass, const VirtualFunctionTableEntry& entry, int stack) {
         mGuards.clear();
         mpClass             = &vclass;
         mpEntry             = &entry;
         mInstructionPointer = entry.mInstruction;
         mStackBase          = stack - entry.mArguments;
      }

      void jump(int address) {
         mInstructionPointer = address;
      }

      Guards                           mGuards;
      const VirtualClass*              mpClass;
      const VirtualFunctionTableEntry* mpEntry;
      int                              mInstructionPointer;
      int                              mStackBase;
   };

   typedef std::stack<VirtualCall> CallStack;
   typedef std::map<std::string, callbackfnc> Natives;
   typedef std::map<void*, VirtualObjectReference> NativeObjectMap;

   enum State { eInit, eRunning, eFinalizing, eReturn, eDestruct };

 // execution
   void execute(const VirtualClass& vclass, const VirtualFunctionTableEntry& entry);
   void execute(const VirtualClass& vclass, const VirtualInstruction& instruction);

   Variant pop();

 // exception
   void throwException(const std::string& exceptionname, const std::string& reason = "");
   bool handleException(const VirtualException& exception);
   void displayException(const VirtualException& exception);

 // class loading
   VirtualClass* doLoadClass(const std::string& classname);
   void          classLoaded(VirtualClass* pclass);
   void          createClass(const VirtualClass& aclass);

 // stack operations
   void shrinkStack(int newsize);

   VirtualContext&               mContext;
   VirtualCompileCallback        mCallback;
   Compiler                      mCompiler;
   GarbageCollector              mGC;
   Stack                         mStack;
   CallStack                     mCallStack;
   VirtualCall                   mCall;
   VirtualObjectReference        mException;
   VirtualMachineObjectObserver  mObjectObserver;
   Natives                       mNatives;
   NativeObjectMap               mNativeObjects;
   State                         mState;
   bool                          mRetVal;
   bool                          mLoaded;
};

#endif // VM_H_
