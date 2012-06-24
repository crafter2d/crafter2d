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

// When testing with Visual Leak Detecter, uncomment the next line
// #include <vld.h>
#include <iostream>

#include "core/defines.h"
#include "core/smartptr/autoptr.h"
#include "core/string/char.h"

#include "script/compiler/compiler.h"
#include "script/common/literal.h"

#include "virtualarrayexception.h"
#include "virtualfunctionnotfoundexception.h"
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
   std::cout << accessor.getString(1) << std::endl;
}

void Console_print(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   std::cout << accessor.getString(1);
}

void ClassLoader_doLoadClass(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   const std::string& classname = accessor.getString(1);

   accessor.setResult(machine.loadClass(classname));
}

void Class_doNewInstance(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   VirtualObjectReference& classobject = accessor.getObject(1);

   std::string name = classobject->getMember(0).asString();
   VirtualObjectReference object = machine.instantiate(name);

   accessor.setResult(object);
}

void Function_doInvoke(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   VirtualObjectReference& instance = accessor.getObject(1);

   std::string fncname = thisobject->getMember(0).asString();

   machine.execute(instance, fncname);
}

void Throwable_fillCallStack(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   std::string callstack = machine.buildCallStack();

   accessor.setResult(callstack);
}

void InternalArray_resize(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualArrayReference& thisobject = accessor.getArray(0);

   int newsize = accessor.getInt(1);

   thisobject->resize(newsize);
}

void InternalString_equals(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const std::string& thisstring = accessor.getString(0);
   const std::string& thatstring = accessor.getString(1);

   accessor.setResult(thisstring == thatstring);
}

void InternalString_subString(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const std::string& thisstring = accessor.getString(0);
   
   int pos = accessor.getInt(1);
   int len = accessor.getInt(2);

   accessor.setResult(thisstring.substr(pos, len));
}

void InternalString_length(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const std::string& thisstring = accessor.getString(0);

   accessor.setResult((int) thisstring.length());
}

void InternalString_getChar(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const std::string& thisstring = accessor.getString(0);

   int index = accessor.getInt(1);

   accessor.setResult(thisstring[index]);
}

void Char_isAlphaNum(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   char c = accessor.getChar(0);

   accessor.setResult(Char::isAlphaNum(c));
}

void Char_isAlpha(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   char c = accessor.getChar(0);

   accessor.setResult(Char::isAlpha(c));
}

void Char_isDigit(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   char c = accessor.getChar(0);

   accessor.setResult(Char::isDigit(c));
}

void Char_isWhitespace(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   char c = accessor.getChar(0);

   accessor.setResult(Char::isWhitespace(c));
}

VirtualMachine::VirtualMachine(VirtualContext& context):
   mContext(context),
   mCallback(*this),
   mCompiler(),
   mGC(),
   mStack(),
   mCallStack(),
   mCall(),
   mObjectObserver(*this),
   mNatives(),
   mpArrayClass(NULL),
   mpStringClass(NULL),
   mState(eRunning),
   mLoaded(false)
{
   mCompiler.setCallback(mCallback);

   mNatives.insert(std::pair<std::string, callbackfnc>("Console_println", Console_println));
   mNatives.insert(std::pair<std::string, callbackfnc>("Console_print", Console_print));
   mNatives.insert(std::pair<std::string, callbackfnc>("ClassLoader_doLoadClass", ClassLoader_doLoadClass));
   mNatives.insert(std::pair<std::string, callbackfnc>("Class_doNewInstance", Class_doNewInstance));
   mNatives.insert(std::pair<std::string, callbackfnc>("Function_doInvoke", Function_doInvoke));
   mNatives.insert(std::pair<std::string, callbackfnc>("Throwable_fillCallStack", Throwable_fillCallStack));
   mNatives.insert(std::pair<std::string, callbackfnc>("InternalArray_resize", InternalArray_resize));
   mNatives.insert(std::pair<std::string, callbackfnc>("InternalString_equals", InternalString_equals));
   mNatives.insert(std::pair<std::string, callbackfnc>("InternalString_length", InternalString_length));
   mNatives.insert(std::pair<std::string, callbackfnc>("InternalString_subString", InternalString_subString));
   mNatives.insert(std::pair<std::string, callbackfnc>("InternalString_getChar", InternalString_getChar));
   mNatives.insert(std::pair<std::string, callbackfnc>("Char_isAlphaNum", Char_isAlphaNum));
   mNatives.insert(std::pair<std::string, callbackfnc>("Char_isAlpha", Char_isAlpha));
   mNatives.insert(std::pair<std::string, callbackfnc>("Char_isDigit", Char_isDigit));
   mNatives.insert(std::pair<std::string, callbackfnc>("Char_isWhitespace", Char_isWhitespace));
}

VirtualMachine::~VirtualMachine()
{
   // set destruct state, native object notifies vm when it is destructed
   // resulting in double delete.
   mState = eDestruct;
   mNativeObjects.clear();
   mNatives.clear();
   mCompiler.cleanUp();
}

// - Initialization

void VirtualMachine::initialize()
{
   // preload some common classes
   loadClass("system.Object");
   loadClass("system.InternalArray");
   loadClass("system.InternalString");

   // resolve the internal classes
   mpArrayClass = &mContext.mClassTable.resolve("system.InternalArray");
   mpStringClass = &mContext.mClassTable.resolve("system.InternalString");

   // pre-compile utility classes
   loadClass("system.ClassLoader");
   loadClass("system.System");

   mState = eRunning;
   mLoaded = true;

   // register the loaded classes with the ClassLoader instance
   std::vector<VirtualClass*> classes = mContext.mClassTable.asArray();
   for ( std::size_t index = 0; index < classes.size(); index++ )
   {
      VirtualClass* pclass = classes[index];
      createClass(*pclass);
   }
}

// - Loading

bool VirtualMachine::loadClass(const std::string& classname)
{
   return doLoadClass(classname) != NULL;
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
   VirtualObjectReference object = instantiate(classname);
   if ( object.isNull() )
   {
      return false;
   }

   execute(object, function);
   
   // for now run the garbage collector here. have to find the right spot for it.
   mGC.gc(*this);

   return true;
}

void VirtualMachine::execute(const VirtualObjectReference& object, const std::string& function)
{
   const VirtualClass& vclass = object->getClass();
   const VirtualFunctionTableEntry* pentry = vclass.getVirtualFunctionTable().findByName(function);
   if ( pentry == NULL )
   {
      throw new VirtualFunctionNotFoundException(object->getClass().getName(), function);
   }

   Variant objectvariant(object);
   if ( pentry->mArguments > 1 )
      mStack.insert(mStack.begin() + mStack.size() - (pentry->mArguments - 1), objectvariant);
   else
      mStack.push_back(objectvariant);

   execute(vclass, *pentry);
   
   // for now run the garbage collector here. have to find the right spot for it.
   mGC.gc(*this);
}

void VirtualMachine::execute(const VirtualClass& vclass, const VirtualFunctionTableEntry& entry)
{
   mCallStack.push(mCall);
   mCall.start(vclass, entry, mStack.size());

   const VirtualInstructionTable& instructions = mContext.mInstructions;

   while ( mState != eReturn )
   {
      try
      {
         const VirtualInstruction& inst = instructions[mCall.mInstructionPointer++];

         execute(vclass, inst);
      }
      catch ( VirtualFunctionNotFoundException* pfuncexception)
      {
         VirtualException* pexception = new VirtualException(instantiate("system.NoSuchFunctionException"));
      }
      catch ( VirtualArrayException* parrayexception )
      {
         VirtualException* pexception = new VirtualException(instantiateArrayException(*parrayexception));

         handleException(pexception);

         delete pexception;
      }
      catch ( VirtualException* pexception )
      {
         handleException(pexception);
      }
   }

   if ( mRetVal )
   {
      Variant result = mStack.back();
      shrinkStack(mCall.mStackBase);
      mStack.push_back(result);
   }
   else
   {
      shrinkStack(mCall.mStackBase);
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
            int arguments = mStack.back().asInt();

            const Variant& object = mStack[mStack.size() - arguments - 1];
            ASSERT(object.isObject());

            if ( !object.asObject()->hasNativeObject() )
            {
               const std::string& fnc = mContext.mLiteralTable[instruction.getArgument()].getValue().asString();

               VirtualStackAccessor accessor(mStack);
               (*mNatives[fnc])(*this, accessor);

               mStack.pop_back(); // pop argument count
            }
         }
         break;
      case VirtualInstruction::eNewArray:
         {
            // n sizes
            // NewArray dimension

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
            if ( object.isObject() )
            {
               const VirtualClass& theclass = object.asObject()->getClass();
               const VirtualFunctionTableEntry& entry = theclass.getVirtualFunctionTable()[instruction.getArgument()];

               execute(theclass, entry);
            }
            else if ( object.isArray() )
            {
               ASSERT_PTR(mpArrayClass);
               const VirtualFunctionTableEntry& entry = mpArrayClass->getVirtualFunctionTable()[instruction.getArgument()];

               execute(*mpArrayClass, entry);
            }
            else if ( object.isString() )
            {
               ASSERT_PTR(mpStringClass);
               const VirtualFunctionTableEntry& entry = mpStringClass->getVirtualFunctionTable()[instruction.getArgument()];

               execute(*mpStringClass, entry);
            }
            else
            {
               ASSERT(object.isEmpty());
               throwException("system.NullPointerException");
            }
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

            Natives::iterator it = mNatives.find(fnc);
            if ( it == mNatives.end() )
            {
               throwException("system.NativeFunctionNotFoundException", fnc);
            }

            VirtualStackAccessor accessor(mStack);
            (*it->second)(*this, accessor);

            mStack.pop_back(); // pop the argument count
            if ( accessor.hasResult() )
               mStack.push_back(accessor.getResult());
         }
         break;
      case VirtualInstruction::eCallStatic:
         {
            int classlit = mStack.back().asInt(); mStack.pop_back();
            int arguments = mStack.back().asInt(); mStack.pop_back();

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
      case VirtualInstruction::eChar2String:
         {
            mStack.back().char2string();
         }
         break;
      case VirtualInstruction::eBoolean2String:
         {
            mStack.back().boolean2string();
         }
         break;

      // - Boolean interface

      case VirtualInstruction::eCmpEqBool:
         {
            bool right = mStack.back().asInt(); mStack.pop_back();
            bool left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left == right));
         }
         break;
      case VirtualInstruction::eCmpNeqBool:
         {
            bool right = mStack.back().asInt(); mStack.pop_back();
            bool left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left != right));
         }
         break;

      // - Integer interface

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
               throwException("system.DivideByZeroException");
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
      case VirtualInstruction::eIntNegate:
         {
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(-left));
         }
         break;
      case VirtualInstruction::eCmpEqInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left == right));
         }
         break;
      case VirtualInstruction::eCmpNeqInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left != right));
         }
         break;
      case VirtualInstruction::eCmpLtInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left < right));
         }
         break;
      case VirtualInstruction::eCmpLeInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left <= right));
         }
         break;
      case VirtualInstruction::eCmpGtInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left > right));
         }
         break;
      case VirtualInstruction::eCmpGeInt:
         {
            int right = mStack.back().asInt(); mStack.pop_back();
            int left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left >= right));
         }
         break;

      // - Real interface

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
      case VirtualInstruction::eCmpEqReal:
         {
            double right = mStack.back().asReal(); mStack.pop_back();
            double left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left == right));
         }
         break;
      case VirtualInstruction::eCmpNeqReal:
         {
            double right = mStack.back().asReal(); mStack.pop_back();
            double left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left != right));
         }
         break;
      case VirtualInstruction::eCmpLtReal:
         {
            double right = mStack.back().asReal(); mStack.pop_back();
            double left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left < right));
         }
         break;
      case VirtualInstruction::eCmpLeReal:
         {
            double right = mStack.back().asReal(); mStack.pop_back();
            double left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left <= right));
         }
         break;
      case VirtualInstruction::eCmpGtReal:
         {
            int right = mStack.back().asReal(); mStack.pop_back();
            int left  = mStack.back().asReal(); mStack.pop_back();

            mStack.push_back(Variant(left > right));
         }
         break;
      case VirtualInstruction::eCmpGeReal:
         {
            double right = mStack.back().asInt(); mStack.pop_back();
            double left  = mStack.back().asInt(); mStack.pop_back();

            mStack.push_back(Variant(left >= right));
         }
         break;

      // - Char interface
      case VirtualInstruction::eAddChar:
         {
            char right = mStack.back().asChar(); mStack.pop_back();
            std::string left = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left + right));
         }
         break;
      case VirtualInstruction::eCmpEqChar:
         {
            char right = mStack.back().asChar(); mStack.pop_back();
            char left  = mStack.back().asChar(); mStack.pop_back();

            mStack.push_back(Variant(left == right));
         }
         break;
      case VirtualInstruction::eCmpNeqChar:
         {
            char right = mStack.back().asChar(); mStack.pop_back();
            char left  = mStack.back().asChar(); mStack.pop_back();

            mStack.push_back(Variant(left != right));
         }
         break;

      // - String interface

      case VirtualInstruction::eAddStr:
         {
            std::string right = mStack.back().asString(); mStack.pop_back();
            std::string left = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left + right));
         }
         break;
      case VirtualInstruction::eCmpEqStr:
         {
            std::string right = mStack.back().asString(); mStack.pop_back();
            std::string left  = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left == right));
         }
         break;
      case VirtualInstruction::eCmpNeqStr:
         {
            std::string right = mStack.back().asString(); mStack.pop_back();
            std::string left  = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left != right));
         }
         break;
      case VirtualInstruction::eCmpLeStr:
         {
            std::string right = mStack.back().asString(); mStack.pop_back();
            std::string left  = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left <= right));
         }
         break;
      case VirtualInstruction::eCmpLtStr:
         {
            std::string right = mStack.back().asString(); mStack.pop_back();
            std::string left  = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left < right));
         }
         break;
      case VirtualInstruction::eCmpGeStr:
         {
            std::string right = mStack.back().asString(); mStack.pop_back();
            std::string left  = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left > right));
         }
         break;
      case VirtualInstruction::eCmpGtStr:
         {
            std::string right = mStack.back().asString(); mStack.pop_back();
            std::string left  = mStack.back().asString(); mStack.pop_back();

            mStack.push_back(Variant(left >= right));
         }
         break;

      // Object/array interface

      case VirtualInstruction::eCmpEqObj:
         {
            VirtualObjectReference right = mStack.back().asObject(); mStack.pop_back();
            VirtualObjectReference left  = mStack.back().asObject(); mStack.pop_back();

            mStack.push_back(Variant(left.ptr() == right.ptr()));
         }
         break;
      case VirtualInstruction::eCmpNeqObj:
         {
            VirtualObjectReference right = mStack.back().asObject(); mStack.pop_back();
            VirtualObjectReference left  = mStack.back().asObject(); mStack.pop_back();

            mStack.push_back(Variant(left.ptr() != right.ptr()));
         }
         break;
      case VirtualInstruction::eCmpEqAr:
         {
            VirtualArrayReference right = mStack.back().asArray(); mStack.pop_back();
            VirtualArrayReference left  = mStack.back().asArray(); mStack.pop_back();

            mStack.push_back(Variant(left.ptr() == right.ptr()));
         }
         break;
      case VirtualInstruction::eCmpNeqAr:
         {
            VirtualArrayReference right = mStack.back().asArray(); mStack.pop_back();
            VirtualArrayReference left  = mStack.back().asArray(); mStack.pop_back();

            mStack.push_back(Variant(left.ptr() != right.ptr()));
         }
         break;

      // nullekes

      case VirtualInstruction::eIsNull:
         {
            bool empty = mStack.back().isEmpty(); mStack.pop_back();

            mStack.push_back(Variant(empty));
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
      case VirtualInstruction::ePushNull:
         mStack.push_back(Variant());
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
         mStack.back().setBool(!mStack.back().asBool());
         break;

      case VirtualInstruction::ePop:
         shrinkStack(mStack.size() - instruction.getArgument());
         break;

      case VirtualInstruction::eInstanceOf:
         {
            const std::string& name = mContext.mLiteralTable[instruction.getArgument()].getValue().asString();
            const VirtualClass* pcompareclass = mContext.mClassTable.find(name);

            Variant object = mStack.back();  mStack.pop_back();
            ASSERT(object.isObject());

            if ( object.asObject()->getClass().isBaseClass(*pcompareclass)
              || object.asObject()->getClass().implements(*pcompareclass) )
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
            else if ( obj.isEmpty() )
            {
               // error!!
               throwException("system.NullPointerException");
            }
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

            const VirtualClass& classloader = mContext.mClassTable.resolve("system.ClassLoader");
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

std::string VirtualMachine::buildCallStack() const
{
   CallStack dump = mCallStack;

   std::string result = "Call stack:\n";
   while ( dump.size() > 1 )
   {
      const VirtualCall& call = dump.top();

      ASSERT_PTR(call.mpEntry);
      result += "- " + call.mpClass->getName() + '.' + call.mpEntry->mName + '(';

      for ( int index = 0; index < call.mpEntry->mArguments; index++ )
      {
         const Variant& value = mStack[call.mStackBase + index];
         result += value.typeAsString() + " = " + value.toString();

         if ( index < call.mpEntry->mArguments - 1 )
         {
            result += ", ";
         }
      }

      result += ")\n";

      dump.pop();
   }

   return result;
}

void VirtualMachine::throwException(const std::string& exceptionname, const std::string& reason)
{
   VirtualObjectReference exception(instantiate(exceptionname, -1));

   if ( reason.length() > 0 )
   {
      mStack.push_back(Variant(reason));
      execute(exception, "setCause");
   }

   throw new VirtualException(exception);
}

bool VirtualMachine::handleException(VirtualException* pexception)
{
   if ( mCall.mGuards.size() > 0 )
   {
      VirtualCall::VirtualGuard& guard = mCall.mGuards.back();
      if ( mCall.mInstructionPointer <= guard.mJumpTo )
      {
         mStack.push_back(Variant(pexception->getException()));

         mCall.jump(guard.mJumpTo);

         return true;
      }
      else
      {
         // exception within a catch handler, so see if there is another try/catch around this one
         mCall.mGuards.pop_back();

         return handleException(pexception);
      }
   }

   // shrink the stack
   shrinkStack(mCall.mStackBase);
   mCall = mCallStack.top();
   mCallStack.pop();

   throw pexception;
}

void VirtualMachine::displayException(const VirtualException& exception)
{
   const VirtualObjectReference& exceptionobject = exception.getException();

   execute(exceptionobject, "getCause");
   execute(exceptionobject, "getCallStack");

   std::string callstack = mStack.back().asString();
   mStack.pop_back();

   std::string cause = mStack.back().asString();
   mStack.pop_back();

   std::cout << cause << std::endl << callstack;
}

// - Object creation

VirtualObjectReference VirtualMachine::instantiate(const std::string& classname, int constructor, void* pnativeobject)
{
   VirtualClass* pclass = doLoadClass(classname);
   if ( pclass == NULL )
   {
      return VirtualObjectReference();
   }

   if ( !pclass->canInstantiate() )
   {
      // not allowed to do instantiate this class (abstract)
      throw std::exception();
   }

   VirtualObjectReference object(pclass->instantiate());
   object->setNativeObject(pnativeobject);
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
   if ( pobject == NULL )
   {
      return VirtualObjectReference();
   }

   NativeObjectMap::iterator it = mNativeObjects.find(pobject);
   if ( it != mNativeObjects.end() )
   {
      // already constructed this object earlier
      ASSERT(it->second->getNativeObject() == pobject);
      VirtualObjectReference& ref = it->second;
      ref->setOwner(owned);
      return ref;
   }

   // construct new instance & remember it
   VirtualObjectReference object(instantiate(classname, -1, pobject));
   if ( !object.isNull() )
   {
      ASSERT(object->hasNativeObject() && object->getNativeObject() == pobject);
      object->setOwner(owned);

      mNativeObjects[pobject] = object;
   }

   return object;
}

VirtualObjectReference VirtualMachine::instantiateShare(const VirtualObjectReference& origin)
{
   VirtualObjectReference share = origin->clone();

   mNativeObjects[share->getNativeObject()] = share;

   return share;
}

VirtualObjectReference VirtualMachine::instantiateArrayException(const VirtualArrayException& e)
{
   VirtualObjectReference result;

   switch ( e.getKind() )
   {
      case VirtualArrayException::eOutOfBounds: 
         {
            result = instantiate("system.ArrayIndexOutOfBoundsException", -1);
            break;
         }
      default:
         UNREACHABLE("Invalid enum value");
   }
   
   return result;
}

VirtualObjectReference VirtualMachine::lookupNative(void* pobject)
{
   NativeObjectMap::iterator it = mNativeObjects.find(pobject);
   if ( it != mNativeObjects.end() )
   {
      ASSERT(it->second->getNativeObject() == pobject);
      return it->second;
   }
   return VirtualObjectReference();
}

VirtualArrayReference VirtualMachine::instantiateArray()
{
   ASSERT_PTR(mpArrayClass);
   VirtualArrayReference ref(mpArrayClass->instantiateArray());

   return ref;
}

// - Native interface

void VirtualMachine::registerNative(VirtualObjectReference& object, void* pnative)
{
   ASSERT(!object->hasNativeObject());
   object->setNativeObject(pnative);

   std::pair<NativeObjectMap::iterator,bool> ret = mNativeObjects.insert(std::pair<void*, VirtualObjectReference>(pnative, object));
   ASSERT(ret.second);
}

void VirtualMachine::unregisterNative(VirtualObjectReference& object)
{
   if ( object.uses() <= 2 )
   {
      ASSERT(object->hasNativeObject());

      // remove the object from the map
      NativeObjectMap::iterator it = mNativeObjects.find(object->getNativeObject());
      ASSERT(it != mNativeObjects.end());
      mNativeObjects.erase(it);

      if ( object->isOwner() )
      {
         const std::string& classname = object->getClass().getNativeClassName();
         std::string fnc = classname + "_destruct";

         mStack.push_back(Variant(object));
         mStack.push_back(Variant(1));

         VirtualStackAccessor accessor(mStack);
         (*mNatives[fnc])(*this, accessor);

         mStack.pop_back();
         mStack.pop_back();
      }
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
         case VirtualInstruction::eNew:
         case VirtualInstruction::eLoadClass:
         case VirtualInstruction::eLoadStatic:
         case VirtualInstruction::eStoreStatic:
         case VirtualInstruction::eLookup:
            lookback++;

         case VirtualInstruction::eNewNative:
         case VirtualInstruction::eCallNative:
         case VirtualInstruction::eLoadLiteral:
         case VirtualInstruction::eInstanceOf:
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

   // execute the static initialization body
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
      object.setClass(mContext.mClassTable.resolve("system.Class"));

      VirtualArrayReference arrayref = object.getMember(1).asArray();
      const VirtualClass& funcclass = mContext.mClassTable.resolve("system.Function");
      for ( int index = 0; index < arrayref->size(); index++ )
      {
         VirtualObjectReference funcref = (*arrayref)[index].asObject();
         VirtualObject& func = *funcref;
         func.setClass(funcclass);
      }

      // notify the ClassLoader to add this class
      const VirtualClass& classloader = mContext.mClassTable.resolve("system.ClassLoader");
      const VirtualFunctionTableEntry& entry = classloader.getVirtualFunctionTable()[4];
      const Variant& classloaderobject = classloader.getStatic(0);

      mStack.push_back(classloaderobject);
      mStack.push_back(Variant(objectref));

      execute(classloader, entry);
   }
}

// - Stack operations

void VirtualMachine::shrinkStack(int newsize)
{
   int diff = mStack.size() - newsize;
   ASSERT(diff >= 0);

   if ( diff > 0 )
   {
      Stack::iterator it = mStack.begin() + newsize + 1;
      for ( ; it != mStack.end(); ++it )
      {
         Variant& variant = *it;
         if ( variant.isObject() )
         {
            VirtualObjectReference& ref = variant.asObject();
            if ( ref.isUnique() || (ref->hasNativeObject() && ref.uses() == 2) )
            {
               mGC.collect(ref);
            }
         }
      }

      mStack.resize(newsize);
   }
}
