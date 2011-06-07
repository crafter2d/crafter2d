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
#include "virtualmachine.h"

#include <iostream>

#include "core/defines.h"

#include "script/compiler/compiler.h"
#include "script/common/literal.h"

#include "virtualinstructiontable.h"
#include "virtualcontext.h"
#include "virtualobject.h"
#include "virtualobjectreference.h"
#include "virtualclass.h"
#include "virtualarrayobject.h"
#include "virtualarrayreference.h"
#include "virtualexception.h"
#include "virtualfunctiontable.h"
#include "virtualfunctiontableentry.h"
#include "virtuallookuptable.h"

void Console_println(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   std::cout << accessor.getString(1);
}

void Class_doNewInstance(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference thisobject = accessor.getThis();
   VirtualObjectReference classobject = accessor.getObject(1);

   std::string name = classobject->getMember(0).asString();
   VirtualObjectReference object = machine.instantiate(name);

   accessor.setResult(object);
}

void Function_doInvoke(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference thisobject = accessor.getThis();
   VirtualObjectReference instance = accessor.getObject(1);

   std::string fncname = thisobject->getMember(0).asString();

   machine.execute(instance, fncname);
}

VirtualMachine::VirtualMachine():
   mContext(),
   mCallback(*this),
   mCompiler(),
   mStack(),
   mCallStack(),
   mCall(),
   mException(NULL),
   mObjectObserver(*this),
   mNatives(),
   mState(eRunning),
   mLoaded(false)
{
   mCompiler.setCallback(mCallback);

   mNatives.insert(std::pair<std::string, callbackfnc>("Console_println", Console_println));
   mNatives.insert(std::pair<std::string, callbackfnc>("Class_doNewInstance", Class_doNewInstance));
   mNatives.insert(std::pair<std::string, callbackfnc>("Function_doInvoke", Function_doInvoke));
}

VirtualMachine::~VirtualMachine()
{
   // set destruct state, native object notifies vm when it is destructed 
   // resulting in double delete.
   mState = eDestruct;
   mNativeObjects.clear();
}

// - Initialization

void VirtualMachine::initialize()
{
   loadClass("System.Object");
   loadClass("System.InternalArray");
   loadClass("System.ClassLoader");
   loadClass("System.System");

   mState = eRunning;
   mLoaded = true;
}

// - Loading
   
bool VirtualMachine::loadClass(const std::string& classname)
{
   return doLoadClass(classname) != NULL;
}

bool VirtualMachine::loadExpression(const std::string& expression)
{
   return true;
}

void VirtualMachine::registerCallback(const std::string& name, callbackfnc callback)
{
   mNatives.insert(std::pair<std::string, callbackfnc>(name, callback));
}

// - Stack access

int VirtualMachine::popInt()
{
   int value = mStack.back().asInt();
   mStack.pop_back();
   return value;
}

double VirtualMachine::popReal()
{
   double value = mStack.back().asReal();
   mStack.pop_back();
   return value;
}

bool VirtualMachine::popBoolean()
{
   bool value = mStack.back().asBool(); 
   mStack.pop_back();
   return value;
}

std::string VirtualMachine::popString()
{
   std::string value = mStack.back().asString();
   mStack.pop_back();
   return value;
}

void VirtualMachine::push(int value)
{
   mStack.push_back(Variant(value));
}

void VirtualMachine::push(double value)
{
   mStack.push_back(Variant(value));
}

void VirtualMachine::push(bool value)
{
   mStack.push_back(Variant(value));
}

void VirtualMachine::push(const std::string& value)
{
   mStack.push_back(Variant(value));
}

void VirtualMachine::push(const VirtualObjectReference& object)
{
  mStack.push_back(Variant(object));
}

// - Execution

bool VirtualMachine::execute(const std::string& classname, const std::string& function)
{
   VirtualClass* pclass = doLoadClass(classname);
   ASSERT_PTR(pclass);
   
   const VirtualFunctionTableEntry* pentry = pclass->getVirtualFunctionTable().findByName(function);
   if ( pentry != NULL )
   {
      mStack.clear();
      
      try
      {
         execute(*pclass, *pentry);
      }
      catch ( VirtualException* pexception )
      {
         std::cout << "Unhandled exception: " << pexception->getException()->getClass().getName() << std::endl;
         return false;
      }
      catch (...)
      {
      }

      return true;
   }

   return false;
}

void VirtualMachine::execute(const VirtualObjectReference& object, const std::string& function)
{
   const VirtualClass& vclass = object->getClass();
   const VirtualFunctionTableEntry* pentry = vclass.getVirtualFunctionTable().findByName(function);
   if ( pentry == NULL )
   {
      throw std::exception();
   }

   Variant objectvariant(object);
   if ( pentry->mArguments > 1 )
      mStack.insert(mStack.begin() + mStack.size() - (pentry->mArguments - 1), objectvariant);
   else
      mStack.push_back(objectvariant);

   execute(vclass, *pentry);
}

void VirtualMachine::execute(const VirtualClass& vclass, const VirtualFunctionTableEntry& entry)
{
   mCallStack.push(mCall);
   mCall.start(entry, mStack.size());

   const VirtualInstructionTable& instructions = mContext.mInstructions;

   while ( mState != eReturn )
   {
      try
      {
         const VirtualInstruction& inst = instructions[mCall.mInstructionPointer++];

         execute(vclass, inst);
      }
      catch ( VirtualException* pexception )
      {
         if ( handleException(*pexception) )
         {
            mException.clear();
         }
         else
         {
            mCall = mCallStack.top();
            mCallStack.pop();
            throw;
         }
      }
      catch ( std::exception& e )
      {
         e.what();
      }
   }

   if ( mRetVal )
   {
      Variant result = mStack.back();
      mStack.resize(mCall.mStackBase);
      mStack.push_back(result);
   }
   else
   {
      mStack.resize(mCall.mStackBase);
   }

   mCall = mCallStack.top();
   mCallStack.pop();

   mState = eRunning;
}

void VirtualMachine::execute(const VirtualClass& vclass, const VirtualInstruction& instruction)
{
   switch ( instruction.getInstruction() )
   {
      case VirtualInstruction::eReserve:
         {
            // reserver 'argument' number of places on stack for arguments/variables
            for ( int index = 0; index < instruction.getArgument(); index++ )
            {
               mStack.push_back(Variant());
            }
         }
         break;
      case VirtualInstruction::eNew:
         {
            const std::string& type = mContext.mLiteralTable[mStack.back().asInt()].getValue().asString();
            mStack.pop_back();

            Variant object(instantiate(type, instruction.getArgument()));

            mStack.push_back(object);
         }
         break;
      case VirtualInstruction::eNewNative:
         {
            const std::string& fnc = mContext.mLiteralTable[instruction.getArgument()].getValue().asString();
            
            int args = mStack.back().asInt();
            
            VirtualStackAccessor accessor(mStack);
            (*mNatives[fnc])(*this, accessor);

            mStack.pop_back(); // pop argument count
         }
         break;
      case VirtualInstruction::eNewArray:
         {
            int arraydimension = instruction.getArgument();

            VirtualArrayReference array = instantiateArray();

            for ( int index = 0; index < arraydimension; index++ )
            {
               Variant size = mStack.back();
               mStack.pop_back();

               array->addLevel(size.asInt());
            }

            mStack.push_back(Variant(array));
         }
         break;
      case VirtualInstruction::eCallSuper:
         {
            int arguments = mStack.back().asInt();
            mStack.pop_back();

            const Variant& object = mStack[mStack.size() - arguments];
            ASSERT(object.isObject());

            const VirtualClass& theclass = vclass.getBaseClass();
            const VirtualFunctionTableEntry& entry = theclass.getVirtualFunctionTable()[instruction.getArgument()];

            execute(theclass, entry);
         }
         break;
      case VirtualInstruction::eCall:
         {
            int arguments = mStack.back().asInt();
            mStack.pop_back();

            const Variant& object = mStack[mStack.size() - arguments]; // find the object to call the method on
            ASSERT(object.isObject());

            const VirtualClass& theclass = object.asObject()->getClass();
            const VirtualFunctionTableEntry& entry = theclass.getVirtualFunctionTable()[instruction.getArgument()];

            execute(theclass, entry);
         }
         break;
      case VirtualInstruction::eCallInterface:
         {
            int arguments = mStack.back().asInt();
            mStack.pop_back();

            const Variant& object = mStack[mStack.size() - arguments]; // find the object to call the method on
            ASSERT(object.isObject());

            const VirtualClass& theclass = object.asObject()->getClass();
            const VirtualFunctionTableEntry& entry = theclass.getVirtualFunctionTable().resolveInterface(instruction.getArgument());

            execute(theclass, entry);
         }
         break;
      case VirtualInstruction::eCallNative:
         {
            const std::string& fnc = mContext.mLiteralTable[instruction.getArgument()].getValue().asString();
            
            VirtualStackAccessor accessor(mStack);
            (*mNatives[fnc])(*this, accessor);

            int args = mStack.back().asInt();
            mStack.resize(mStack.size() - (args + 1));
            if ( accessor.hasResult() )
               mStack.push_back(accessor.getResult());
         }
         break;
      case VirtualInstruction::eCallStatic:
         {
            int arguments = mStack.back().asInt(); mStack.pop_back();
            int classlit = mStack.back().asInt(); mStack.pop_back();

            std::string classname = mContext.mLiteralTable[classlit].getValue().asString();

            const VirtualClass& theclass = mContext.mClassTable.resolve(classname);
            const VirtualFunctionTableEntry& entry = theclass.getVirtualFunctionTable()[instruction.getArgument()];

            execute(theclass, entry);
         }
         break;
      case VirtualInstruction::eRet:
         {
            // remove the function arguments from the stack & place the result back
            if ( !mCall.mGuards.empty() && mCall.mGuards.back().mFinally )
               mState = eFinalizing;
            else
               mState = eReturn;

            mRetVal = instruction.getArgument() == 1;
         }
         break;

      // casting

      case VirtualInstruction::eInt2Real:
         {
            mStack.back().int2real();
         }
         break;
      case VirtualInstruction::eInt2String:
         {
            mStack.back().int2string();
         }
         break;
      case VirtualInstruction::eReal2Int:
         {
            mStack.back().real2int();
         }
         break;
      case VirtualInstruction::eReal2String:
         {
            mStack.back().real2string();
         }
         break;

      // arithmatics

      case VirtualInstruction::eAddInt:
         {
            int right = mStack.back().toInt(); mStack.pop_back();
            int left  = mStack.back().toInt(); mStack.pop_back();

            mStack.push_back(Variant(left + right));
         }
         break;
      case VirtualInstruction::eSubInt:
         {
            int right = mStack.back().toInt(); mStack.pop_back();
            int left  = mStack.back().toInt(); mStack.pop_back();

            mStack.push_back(Variant(left - right));
         }
         break;
      case VirtualInstruction::eMulInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left * right));
         }
         break;
      case VirtualInstruction::eDivInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            if ( right == 0 )
            {
               throwException("DivideByZeroException");
            }

            mStack.push_back(Variant(left / right));
         }
         break;
      case VirtualInstruction::eOrInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left | right));
         }
         break;
      case VirtualInstruction::eXorInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left ^ right));
         }
         break;
      case VirtualInstruction::eAndInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left & right));
         }
         break;
      case VirtualInstruction::eShiftLeftInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left << right));
         }
         break;
      case VirtualInstruction::eShiftRightInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left >> right));
         }
         break;
      case VirtualInstruction::eRemInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left % right));
         }
         break;
      case VirtualInstruction::eAddReal:
         {
            double right = mStack.back().asReal(); mStack.pop_back();
            double left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left + right));
         }
         break;
      case VirtualInstruction::eSubReal:
         {
            double right = mStack.back().asReal(); mStack.pop_back();
            double left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left - right));
         }
         break;
      case VirtualInstruction::eMulReal:
         {
            double right = mStack.back().asReal(); mStack.pop_back();
            double left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left * right));
         }
         break;
      case VirtualInstruction::eDivReal:
         {
            double right = mStack.back().asReal(); mStack.pop_back();
            double left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left / right));
         }
         break;
      case VirtualInstruction::eRemReal:
         {
            int right = (int) mStack.back().asReal(); mStack.pop_back();
            int left  = (int) mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant((double)(left % right)));
         }
         break;
      case VirtualInstruction::eRealNegate:
         {
            double top = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(-top));
         }
         break;
      case VirtualInstruction::eAddStr:
         {
            std::string right = mStack.back().asString(); mStack.pop_back();
            std::string left = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left + right));
         }
         break;

      // comparing

      case VirtualInstruction::eCmpEqual:
         {
            Variant right = mStack.back(); mStack.pop_back();
            Variant left  = mStack.back(); mStack.pop_back();

            mStack.push_back(Variant(left == right));
         }
         break;
      case VirtualInstruction::eCmpNotEqual:
         {
            Variant right = mStack.back(); mStack.pop_back();
            Variant left  = mStack.back(); mStack.pop_back();

            mStack.push_back(Variant(left != right));
         }
         break;
      case VirtualInstruction::eCmpSEqual:
         {
            Variant right = mStack.back(); mStack.pop_back();
            Variant left  = mStack.back(); mStack.pop_back();

            mStack.push_back(Variant(left <= right));
         }
         break;

      case VirtualInstruction::eCmpSmaller:
         {
            Variant right = mStack.back(); mStack.pop_back();
            Variant left  = mStack.back(); mStack.pop_back();

            mStack.push_back(Variant(left < right));
         }
         break;

      case VirtualInstruction::eCmpGreater:
         {
            Variant right = mStack.back(); mStack.pop_back();
            Variant left  = mStack.back(); mStack.pop_back();

            mStack.push_back(Variant(left > right));
         }
         break;

      case VirtualInstruction::eCmpGEqual:
         {
            Variant right = mStack.back(); mStack.pop_back();
            Variant left  = mStack.back(); mStack.pop_back();

            mStack.push_back(Variant(left >= right));
         }
         break;

      // jumps

      case VirtualInstruction::eLookup:
         {
            std::string classname = mContext.mLiteralTable[mStack.back().asInt()].getValue().asString();
            const VirtualLookupTable& table = mContext.mClassTable.resolve(classname).getLookupTable(instruction.getArgument());
            mStack.pop_back();

            int codeindex = table.lookup(mStack.back());
            mStack.pop_back();

            mCall.jump(codeindex);
         }
         break;
      case VirtualInstruction::eJump:
         {
            mCall.jump(instruction.getArgument());
         }
         break;
      case VirtualInstruction::eJumpFalse:
         {
            if ( !mStack.back().asBool() )
            {
               mCall.jump(instruction.getArgument());
            }
            mStack.pop_back();
         }
         break;
      case VirtualInstruction::eJumpTrue:
         {
            if ( mStack.back().asBool() )
            {
               mCall.jump(instruction.getArgument());
            }
            mStack.pop_back();
         }
         break;

      // stack

      case VirtualInstruction::ePush:
         mStack.push_back(Variant(instruction.getArgument()));
         break;
      case VirtualInstruction::ePushTrue:
         mStack.push_back(Variant(true));
         break;
      case VirtualInstruction::ePushFalse:
         mStack.push_back(Variant(false));
         break;
      case VirtualInstruction::ePushThis:
         {
            const Variant& value = mStack[mCall.mStackBase];
            mStack.push_back(value);
         }
         break;
      case VirtualInstruction::eInt0:
         mStack.push_back(Variant(0));
         break;
      case VirtualInstruction::eInt1:
         mStack.push_back(Variant(1));
         break;
      case VirtualInstruction::eInt2:
         mStack.push_back(Variant(2));
         break;
      case VirtualInstruction::eReal0:
         mStack.push_back(Variant(0.0));
         break;
      case VirtualInstruction::eReal1:
         mStack.push_back(Variant(1.0));
         break;
      case VirtualInstruction::eReal2:
         mStack.push_back(Variant(2.0));
         break;
      case VirtualInstruction::eDup:
         mStack.push_back(mStack.back());
         break;
      case VirtualInstruction::eNot:
         {
            //Variant value = Variant(!mStack.back().asBool()); mStack.pop_back();
            //mStack.push_back(value);
            mStack.back() = Variant(!mStack.back().asBool());
         }
         break;

      case VirtualInstruction::ePop:
         for ( int index = 0; index < instruction.getArgument(); index++ )
         {
            mStack.pop_back();
         }
         break;

      case VirtualInstruction::eInstanceOf:
         {
            const std::string& name = mContext.mLiteralTable[instruction.getArgument()].getValue().asString();
            const VirtualClass* pcompareclass = mContext.mClassTable.find(name);

            Variant object = mStack.back();  mStack.pop_back();
            ASSERT(object.isObject());

            if ( object.asObject()->getClass().isBaseClass(*pcompareclass) )
            {
               mStack.push_back(Variant(true));
            }
            else
            {
               mStack.push_back(Variant(false));
            }
         }
         break;

      // exceptions

      case VirtualInstruction::eEnterGuard:
         {
            mCall.mGuards.push_back(VirtualCall::VirtualGuard(instruction.getArgument(), false));
         }
         break;
      case VirtualInstruction::eEnterGuardF:
         {
            mCall.mGuards.push_back(VirtualCall::VirtualGuard(instruction.getArgument(), true));
         }
         break;
      case VirtualInstruction::eLeaveGuard:
         {
            mCall.mGuards.pop_back();

            if ( mState == eFinalizing )
            {
               while ( mCall.mGuards.size() > 0 )
               {
                  VirtualCall::VirtualGuard& guard = mCall.mGuards.back();
                  if ( guard.mFinally )
                  {
                     mCall.jump(guard.mJumpTo);
                     return;
                  }
                  mCall.mGuards.pop_back();
               }

               ASSERT(mCall.mGuards.empty());
               
               mState = eReturn;
            }
         }
         break;
      case VirtualInstruction::eThrow:
         {
            ASSERT(mStack.back().isObject());

            VirtualObjectReference exception = mStack.back().asObject();
            mStack.pop_back();

            throw new VirtualException(exception);
         }
         break;

      // variables

      case VirtualInstruction::eLoad:
         {
            Variant obj = mStack.back();
            mStack.pop_back();
            
            if ( obj.isObject() )
               mStack.push_back((*obj.asObject()).getMember(instruction.getArgument()));
            else if ( obj.isArray() )
               mStack.push_back(Variant(obj.asArray().ptr()->size()));
         }
         break;
      case VirtualInstruction::eStore:
         {
            Variant obj = mStack.back();
            mStack.pop_back();

            ASSERT(obj.isObject());
            Variant value = mStack.back();
            mStack.pop_back();

            obj.asObject()->setMember(instruction.getArgument(), value);
         }
         break;
      case VirtualInstruction::eLoadLocal:
         {
            Variant& value = mStack[mCall.mStackBase + instruction.getArgument()];

            mStack.push_back(value);
         }
         break;
      case VirtualInstruction::eStoreLocal:
         {
            Variant value = mStack.back();
            mStack.pop_back();

            mStack[mCall.mStackBase + instruction.getArgument()] = value;
         }
         break;
      case VirtualInstruction::eLoadArray:
         {
            // stack content:
            // - n indices
            // - array object
            // the actual array object is below the indices in the stack

            Variant& variant = mStack[mStack.size() - instruction.getArgument() - 1];
            ASSERT(variant.isArray());

            VirtualArrayObject* parray = variant.asArray().ptr();
            
            for ( int index = 0; index < instruction.getArgument() - 1; index++ )
            {
               int i = mStack.back().asInt();
               mStack.pop_back();

               parray = parray->at(i).asArray().ptr();
            }

            int i = mStack.back().asInt();
            mStack.pop_back();

            mStack.pop_back(); // pop the array from the stack

            mStack.push_back(parray->at(i));
         }
         break;
      case VirtualInstruction::eStoreArray:
         {
            // stack content:
            // - n indices
            // - array object
            // - value to store

            Variant& variant = mStack[mStack.size() - instruction.getArgument() - 1];
            ASSERT(variant.isArray());

            VirtualArrayObject* parray = variant.asArray().ptr();

            for ( int index = 0; index < instruction.getArgument()-1; index++ )
            {
               int i = mStack.back().asInt();
               mStack.pop_back();

               parray = parray->at(i).asArray().ptr();
            }

            int i = mStack.back().asInt();
            mStack.pop_back();

            mStack.pop_back(); // <-- pop array

            parray->at(i) = mStack.back();
            mStack.pop_back();
         }
         break;
      case VirtualInstruction::eStoreStatic:
         {
            int classlit = mStack.back().asInt();
            mStack.pop_back();

            std::string classname = mContext.mLiteralTable[classlit].getValue().asString();
            VirtualClass& c = mContext.mClassTable.resolve(classname);

            c.setStatic(instruction.getArgument(), mStack.back());
            mStack.pop_back();
         }
         break;
      case VirtualInstruction::eLoadStatic:
         {
            int classlit = mStack.back().asInt();
            mStack.pop_back();

            std::string classname = mContext.mLiteralTable[classlit].getValue().asString();
            const VirtualClass& c = mContext.mClassTable.resolve(classname);

            mStack.push_back(c.getStatic(instruction.getArgument()));
         }
         break;
      case VirtualInstruction::eLoadLiteral:
         {
            const Literal& literal = mContext.mLiteralTable[instruction.getArgument()];
            mStack.push_back(literal.getValue());
         }
         break;
      case VirtualInstruction::eLoadClass:
         {
            Variant name;
            if ( instruction.getArgument() == 1 )
            {
               name.setString(mStack.back().asObject()->getClass().getName());
            }
            else
            {
               name.setString(mContext.mLiteralTable[mStack.back().asInt()].getValue().asString());
            }
            mStack.pop_back();
            
            const VirtualClass& classloader = mContext.mClassTable.resolve("System.ClassLoader");
            const VirtualFunctionTableEntry* pentry = classloader.getVirtualFunctionTable().findByName("findClass");
            const Variant& classloaderobject = classloader.getStatic(0);

            mStack.push_back(classloaderobject);
            mStack.push_back(name);
      
            execute(classloader, *pentry);
         }
         break;
      
      case VirtualInstruction::eNop:
      default:
         break;
   }
}

// - Exception

void VirtualMachine::throwException(const std::string& exceptionname)
{
   const VirtualClass& excclass = mContext.mClassTable.resolve(exceptionname);

   VirtualObjectReference exception(excclass.instantiate());

   throw new VirtualException(exception);
} 

bool VirtualMachine::handleException(const VirtualException& e)
{
   if ( mCall.mGuards.size() > 0 )
   {
      VirtualCall::VirtualGuard& guard = mCall.mGuards.back();
      if ( mCall.mInstructionPointer <= guard.mJumpTo )
      {
         mException = e.getException();

         mStack.push_back(Variant(mException));

         mCall.jump(guard.mJumpTo);

         return true;
      }
      else
      {
         // exception within a catch handler, so see if there is another try/catch around this one
         mCall.mGuards.pop_back();
         
         return handleException(e);
      }
   }
   
   return false;
}

// - Object creation

VirtualObjectReference VirtualMachine::instantiate(const std::string& classname, int constructor)
{
   VirtualClass* pclass = doLoadClass(classname);
   ASSERT_PTR(pclass);

   if ( !pclass->canInstantiate() )
   {
      // not allowed to do instantiate this class (abstract)
      throw std::exception();
   }

   VirtualObjectReference object(pclass->instantiate());
   Variant objectvariant(object);

   {
      // run field initialization expressions
      const VirtualFunctionTableEntry& entry = pclass->getVirtualFunctionTable()[1];
      mStack.push_back(objectvariant);
      execute(*pclass, entry);
   }

   {
      const VirtualFunctionTableEntry* pentry = NULL;

      // call the constructor as coded
      if ( constructor == -1 )
      {
         pentry = pclass->getDefaultConstructor();
      }
      else
      {
         pentry = &pclass->getVirtualFunctionTable()[constructor];
      }

      if (pentry != NULL )
      {
         mStack.insert(mStack.begin() + mStack.size() - (pentry->mArguments - 1), objectvariant);
         execute(*pclass, *pentry);
      }
   }

   return object;
}

VirtualObjectReference VirtualMachine::instantiateNative(const std::string& classname, void* pobject, bool owned)
{
   NativeObjectMap::iterator it = mNativeObjects.find(pobject);
   if ( it != mNativeObjects.end() )
   {
      // validate that it still is the same pointer
      ASSERT(it->second->getNativeObject() == pobject);
      return it->second;
   }
   
   {
      VirtualObjectReference object(instantiate(classname, -1));
      if ( object->hasNativeObject() )
      {
         // TODO: currently new native objects can be constructed during native constructors.
         //       while actually we have an instance already -> waste of CPU & memory
         unregisterNative(object);
      }
      
      object->setNativeObject(pobject);
      object->setOwner(owned);

      mNativeObjects[pobject] = object;

      return object;
   }
}

VirtualArrayReference VirtualMachine::instantiateArray()
{
   VirtualClass& internalarray = mContext.mClassTable.resolve("System.InternalArray");
   VirtualArrayReference ref(internalarray.instantiateArray());

   return ref;
}

// - Native interface

void VirtualMachine::registerNative(VirtualObjectReference& object, void* pnative)
{
   NativeObjectMap::iterator it = mNativeObjects.find(object->getNativeObject());
   if ( it == mNativeObjects.end() )
   {
      mNativeObjects[pnative] = object;

      object->setNativeObject(pnative);
   }
}

void VirtualMachine::unregisterNative(VirtualObjectReference& object)
{
   NativeObjectMap::iterator it = mNativeObjects.find(object->getNativeObject());
   if ( it != mNativeObjects.end() && it->second.isUnique() )
   {
      mNativeObjects.erase(it);
   }
}

// - Callbacks

VirtualClass* VirtualMachine::doLoadClass(const std::string& classname)
{
   VirtualClass* pclass = mContext.mClassTable.find(classname);
   if ( pclass == NULL )
   {
      if ( mCompiler.compile(classname) )
      {
         pclass = mContext.mClassTable.find(classname);
      }
   }
   return pclass;
}

void VirtualMachine::classLoaded(VirtualClass* pclass)
{
   int offset = mContext.mInstructions.size();

   mContext.mClassTable.insert(pclass);
   mContext.mInstructions.add(pclass->getInstructions());

   pclass->offsetCode(offset);

   int lookback = 0;

   for ( int index = offset; index < mContext.mInstructions.size(); index++ )
   {
      VirtualInstruction& instruction = mContext.mInstructions[index];

      switch ( instruction.getInstruction() )
      {
         case VirtualInstruction::eCallStatic:
            lookback++;

         case VirtualInstruction::eNew:
         case VirtualInstruction::eLoadClass:
         case VirtualInstruction::eLoadStatic:
         case VirtualInstruction::eStoreStatic:
         case VirtualInstruction::eLookup:
            lookback++;

         case VirtualInstruction::eNewNative:
         case VirtualInstruction::eCallNative:
         case VirtualInstruction::eLoadLiteral:         
            {
               VirtualInstruction& previous = mContext.mInstructions[index - lookback];
               if ( previous.getInstruction() != VirtualInstruction::ePushThis )
               {
                  const Literal& literal = mCompiler.lookupLiteral(previous.getArgument());

                  int i = mContext.mLiteralTable.indexOf(literal);
                  if ( i == mContext.mLiteralTable.size() )
                  {
                     i = mContext.mLiteralTable.insert(literal.clone());
                  }

                  previous.setArgument(i);
               }

               lookback = 0;
            }
            break;

         default:
            break;
      }
   }

   if ( pclass->hasBaseName() )
   {
      std::string base = pclass->getBaseName();
      const VirtualClass& baseclass = mContext.mClassTable.resolve(base);
      
      pclass->setBaseClass(baseclass);

      VirtualFunctionTable& vtable = pclass->getVirtualFunctionTable();
      vtable.merge(baseclass.getVirtualFunctionTable());
      vtable.offset(offset);
   }

   createClass(*pclass);

   /*
   std::string code = pclass->getInstructions().toString(mContext.mLiteralTable);

   std::cout << "=====================================" << std::endl;
   std::cout << qPrintable(pclass->getName()) << std::endl;
   std::cout << qPrintable(code) << std::endl << std::endl;*/

   VirtualFunctionTableEntry& entry = pclass->getVirtualFunctionTable()[0];
   execute(*pclass, entry);
}

void VirtualMachine::createClass(const VirtualClass& aclass)
{  
   if ( mLoaded )
   {
      VirtualObjectReference objectref = aclass.getClassObject();

      // resolve the virtual classes
      VirtualObject& object = *objectref;
      object.setClass(mContext.mClassTable.resolve("System.Class"));

      VirtualArrayReference arrayref = object.getMember(1).asArray();
      const VirtualClass& funcclass = mContext.mClassTable.resolve("System.Function");
      for ( int index = 0; index < arrayref->size(); index++ )
      {
         VirtualObjectReference funcref = (*arrayref)[index].asObject();
         VirtualObject& func = *funcref;
         func.setClass(funcclass);
      }

      // notify the ClassLoader to add this class
      const VirtualClass& classloader = mContext.mClassTable.resolve("System.ClassLoader");
      const VirtualFunctionTableEntry& entry = classloader.getVirtualFunctionTable()[4];
      const Variant& classloaderobject = classloader.getStatic(0);

      mStack.push_back(classloaderobject);
      mStack.push_back(Variant(objectref));
      
      execute(classloader, entry);
   }
}
