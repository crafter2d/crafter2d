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

#include "core/memory/memorypool.h"
#include "core/defines.h"

#include "script/compiler/compiler.h"
#include "script/gc/garbagecollector.h"

#include "virtualcompilecallback.h"
#include "virtualfunctiontableentry.h"
#include "virtualcontext.h"
#include "virtualstack.h"
#include "vminterface.h"

class ClassRegistry;
class String;
class Variant;
class VirtualArrayException;
class VirtualInstruction;
class VirtualFunctionTableEntry;
class VirtualException;
class VirtualObject;
class VirtualStackAccessor;

class SCRIPT_API VirtualMachine
{
public:
   explicit VirtualMachine(VirtualContext& context);
   ~VirtualMachine();

 // initialization
   void initialize();
   
 // loading
   bool loadClass(const String& classname);
   void mergeClassRegistry(const ClassRegistry& registry);

 // stack access
   int popInt();
   double popReal();
   bool popBoolean();
   String popString();

   void push(int value);
   void push(double value);
   void push(bool value);
   void push(const String& value);
   void push(VirtualObject& object);

   void addRootObject(VirtualObject& object);

 // execution
   bool execute(const String& classname, const String& function);
   void execute(VirtualObject& object, const String& function);

 // exception handling
   String buildCallStack() const;
   void displayException(VirtualException& exception);

 // object instantation
   VirtualObject*    instantiate(const String& classname, int constructor = -1, void* pobject = NULL);
   VirtualObject*    instantiateNative(const String& classname, void* pobject, bool owned = true);
   VirtualArray*     instantiateArray();
   void              release(VirtualObject& object);

 // observing
   VirtualObject*    lookupNative(void* pobject);
   void              registerNative(VirtualObject& object, void* pnative);
   void              unregisterNative(VirtualObject& object);

private:
   friend class VirtualCompileCallback;
   friend class GarbageCollector;

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

   typedef std::vector<VirtualObject*> Objects;
   typedef std::vector<VMInterface::CallbackFnc> Callbacks;
   typedef std::stack<VirtualCall> CallStack;
   typedef std::map<void*, VirtualObject*> NativeObjectMap;

   enum State { eInit, eRunning, eFinalizing, eReturn, eDestruct };

 // execution
   void execute(const VirtualClass& vclass, const VirtualFunctionTableEntry& entry);
   void execute(const VirtualClass& vclass, const VirtualInstruction& instruction);
   
 // exception
   VirtualObject& instantiateArrayException(const VirtualArrayException& e);
   void           throwException(const String& exceptionname, const String& reason);
   bool           handleException(VirtualException* pexception);

 // class loading
   VirtualClass* doLoadClass(const String& classname);
   void          classLoaded(VirtualClass* pclass);
   void          createClass(const VirtualClass& aclass);

 // stack operations
   void shrinkStack(int newsize);

   VirtualContext&               mContext;
   VirtualCompileCallback        mCallback;
   Compiler                      mCompiler;
   Objects                       mRootObjects;
   MemoryPool<VirtualObject>     mObjectCache;
   GarbageCollector              mGC;
   VirtualStack                  mStack;
   CallStack                     mCallStack;
   VirtualCall                   mCall;
   Callbacks                     mCallbacks;
   NativeObjectMap               mNativeObjects;
   State                         mState;
   VirtualClass*                 mpArrayClass;
   VirtualClass*                 mpStringClass;
   bool                          mRetVal;
   bool                          mLoaded;
};

#endif // VM_H_
