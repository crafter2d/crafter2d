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
   VirtualObject& thisobject = accessor.getThis();
   const std::string& classname = accessor.getString(1);

   accessor.setResult(machine.loadClass(classname));
}

void Class_doNewInstance(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();
   VirtualObject& classobject = accessor.getObject(1);

   std::string name = classobject.getMember(0).asString();
   VirtualObject* pobject = machine.instantiate(name);

   accessor.setResult(*pobject);
}

void Function_doInvoke(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();
   VirtualObject& instance = accessor.getObject(1);

   std::string fncname = thisobject.getMember(0).asString();

   machine.execute(instance, fncname);
}

void Throwable_fillCallStack(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

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
   mObjects(),
   mRootObjects(),
   mGC(),
   mStack(),
   mCallStack(),
   mCall(),
   mNatives(),
   mNativeObjects(),
   mState(eInit),
   mpArrayClass(NULL),
   mpStringClass(NULL),
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
   // clear the garbage collector
   mGC.gc(*this);

   // set destruct state, native object notifies vm when it is destructed
   // resulting in double delete.
   mState = eDestruct;
   mNatives.clear();
   mNativeObjects.clear();
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
   return mStack.popInt();
}

double VirtualMachine::popReal()
{
   return mStack.popReal();
}

bool VirtualMachine::popBoolean()
{
   return mStack.popBool();
}

std::string VirtualMachine::popString()
{
   return mStack.popString();
}

void VirtualMachine::push(int value)
{
   mStack.pushInt(value);
}

void VirtualMachine::push(double value)
{
   mStack.pushReal(value);
}

void VirtualMachine::push(bool value)
{
   mStack.pushBool(value);
}

void VirtualMachine::push(const std::string& value)
{
   mStack.pushString(value);
}

void VirtualMachine::push(VirtualObject& object)
{
  mStack.pushObject(object);
}

void VirtualMachine::addRootObject(VirtualObject& object)
{
   mRootObjects.push_back(&object);
}

// - Execution

bool VirtualMachine::execute(const std::string& classname, const std::string& function)
{
   VirtualObject* pvirtualobject = instantiate(classname);
   if ( pvirtualobject == NULL )
   {
      return false;
   }

   execute(*pvirtualobject, function);
   
   // for now run the garbage collector here. have to find the right spot for it.
   mGC.gc(*this);

   return true;
}

void VirtualMachine::execute(VirtualObject& object, const std::string& function)
{
   const VirtualClass& vclass = object.getClass();
   const VirtualFunctionTableEntry* pentry = vclass.getVirtualFunctionTable().findByName(function);
   if ( pentry == NULL )
   {
      throw new VirtualFunctionNotFoundException(object.getClass().getName(), function);
   }

   Variant objectvariant(object);
   if ( pentry->mArguments > 1 )
      mStack.insert(mStack.size() - (pentry->mArguments - 1), objectvariant);
   else
      mStack.push(objectvariant);

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
         VirtualException* pexception = new VirtualException(*instantiate("system.NoSuchFunctionException"));

         handleException(pexception);

         delete pexception;
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
      mStack.push(result);
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
               mStack.push(Variant());
            }
         }
         break;
      case VirtualInstruction::eNew:
         {
            const std::string& type = mContext.mLiteralTable[mStack.popInt()].getValue().asString();

            VirtualObject* pobject = instantiate(type, instruction.getArgument());
            ASSERT_PTR(pobject);

            mStack.pushObject(*pobject);
         }
         break;
      case VirtualInstruction::eNewNative:
         {            
            int arguments = mStack.back().asInt();

            const Variant& object = mStack[mStack.size() - arguments - 1];
            ASSERT(object.isObject());

            if ( !object.asObject().hasNativeObject() )
            {
               const std::string& fnc = mContext.mLiteralTable[instruction.getArgument()].getValue().asString();

               VirtualStackAccessor accessor(mStack);
               (*mNatives[fnc])(*this, accessor);

               mStack.pop(); // pop argument count
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
               int size = mStack.popInt();

               array->addLevel(size);
            }

            mStack.pushArray(array);
         }
         break;
      case VirtualInstruction::eCallSuper:
         {
            int arguments = mStack.popInt();

            const Variant& object = mStack[mStack.size() - arguments];
            ASSERT(object.isObject());

            const VirtualClass& theclass = vclass.getBaseClass();
            const VirtualFunctionTableEntry& entry = theclass.getVirtualFunctionTable()[instruction.getArgument()];

            execute(theclass, entry);
         }
         break;
      case VirtualInstruction::eCall:
         {
            int arguments = mStack.popInt();
            
            const Variant& object = mStack[mStack.size() - arguments]; // find the object to call the method on
            if ( object.isObject() )
            {
               const VirtualClass& theclass = object.asObject().getClass();
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
            int arguments = mStack.popInt();

            const Variant& object = mStack[mStack.size() - arguments]; // find the object to call the method on
            ASSERT(object.isObject());

            const VirtualClass& theclass = object.asObject().getClass();
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

            mStack.pop(); // pop the argument count
            if ( accessor.hasResult() )
               mStack.push(accessor.getResult());
         }
         break;
      case VirtualInstruction::eCallStatic:
         {
            int classlit = mStack.popInt();
            int arguments = mStack.popInt();

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
            bool right = mStack.popBool();
            bool left  = mStack.popBool();

            mStack.pushBool(left == right);
         }
         break;
      case VirtualInstruction::eCmpNeqBool:
         {
            bool right = mStack.popBool();
            bool left  = mStack.popBool();

            mStack.pushBool(left != right);
         }
         break;

      // - Integer interface

      case VirtualInstruction::eAddInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left + right);
         }
         break;
      case VirtualInstruction::eSubInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left - right);
         }
         break;
      case VirtualInstruction::eMulInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left * right);
         }
         break;
      case VirtualInstruction::eDivInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            if ( right == 0 )
            {
               throwException("system.DivideByZeroException");
            }

            mStack.pushInt(left / right);
         }
         break;
      case VirtualInstruction::eOrInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left | right);
         }
         break;
      case VirtualInstruction::eXorInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left ^ right);
         }
         break;
      case VirtualInstruction::eAndInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left & right);
         }
         break;
      case VirtualInstruction::eShiftLeftInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left << right);
         }
         break;
      case VirtualInstruction::eShiftRightInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left >> right);
         }
         break;
      case VirtualInstruction::eRemInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushInt(left % right);
         }
         break;
      case VirtualInstruction::eIntNegate:
         {
            int left = mStack.popInt();

            mStack.pushInt(-left);
         }
         break;
      case VirtualInstruction::eCmpEqInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushBool(left == right);
         }
         break;
      case VirtualInstruction::eCmpNeqInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushBool(left != right);
         }
         break;
      case VirtualInstruction::eCmpLtInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushBool(left < right);
         }
         break;
      case VirtualInstruction::eCmpLeInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushBool(left <= right);
         }
         break;
      case VirtualInstruction::eCmpGtInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushBool(left > right);
         }
         break;
      case VirtualInstruction::eCmpGeInt:
         {
            int right = mStack.popInt();
            int left  = mStack.popInt();

            mStack.pushBool(left >= right);
         }
         break;

      // - Real interface

      case VirtualInstruction::eAddReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushReal(left + right);
         }
         break;
      case VirtualInstruction::eSubReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushReal(left - right);
         }
         break;
      case VirtualInstruction::eMulReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushReal(left * right);
         }
         break;
      case VirtualInstruction::eDivReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushReal(left / right);
         }
         break;
      case VirtualInstruction::eRemReal:
         {
            int right = (int) mStack.popReal();
            int left  = (int) mStack.popReal();

            mStack.pushReal((double)(left % right));
         }
         break;
      case VirtualInstruction::eRealNegate:
         {
            double top = mStack.popReal();

            mStack.pushReal(-top);
         }
         break;
      case VirtualInstruction::eCmpEqReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushBool(left == right);
         }
         break;
      case VirtualInstruction::eCmpNeqReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushBool(left != right);
         }
         break;
      case VirtualInstruction::eCmpLtReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushBool(left < right);
         }
         break;
      case VirtualInstruction::eCmpLeReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushBool(left <= right);
         }
         break;
      case VirtualInstruction::eCmpGtReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushBool(left > right);
         }
         break;
      case VirtualInstruction::eCmpGeReal:
         {
            double right = mStack.popReal();
            double left  = mStack.popReal();

            mStack.pushBool(left >= right);
         }
         break;

      // - Char interface
      case VirtualInstruction::eAddChar:
         {
            char right = mStack.popChar();
            std::string left = mStack.back().asString();

            mStack.pushString(left + right);
         }
         break;
      case VirtualInstruction::eCmpEqChar:
         {
            char right = mStack.popChar();
            char left  = mStack.popChar();

            mStack.pushBool(left == right);
         }
         break;
      case VirtualInstruction::eCmpNeqChar:
         {
            char right = mStack.popChar();
            char left  = mStack.popChar();

            mStack.pushBool(left != right);
         }
         break;

      // - String interface

      case VirtualInstruction::eAddStr:
         {
            std::string right = mStack.popString();
            std::string left = mStack.popString();

            mStack.pushString(left + right);
         }
         break;
      case VirtualInstruction::eCmpEqStr:
         {
            std::string right = mStack.popString();
            std::string left = mStack.popString();

            mStack.pushBool(left == right);
         }
         break;
      case VirtualInstruction::eCmpNeqStr:
         {
            std::string right = mStack.popString();
            std::string left = mStack.popString();

            mStack.pushBool(left != right);
         }
         break;
      case VirtualInstruction::eCmpLeStr:
         {
            std::string right = mStack.popString();
            std::string left = mStack.popString();

            mStack.pushBool(left <= right);
         }
         break;
      case VirtualInstruction::eCmpLtStr:
         {
            std::string right = mStack.popString();
            std::string left = mStack.popString();

            mStack.pushBool(left < right);
         }
         break;
      case VirtualInstruction::eCmpGeStr:
         {
            std::string right = mStack.popString();
            std::string left = mStack.popString();

            mStack.pushBool(left > right);
         }
         break;
      case VirtualInstruction::eCmpGtStr:
         {
            std::string right = mStack.popString();
            std::string left = mStack.popString();

            mStack.pushBool(left >= right);
         }
         break;

      // Object/array interface

      case VirtualInstruction::eCmpEqObj:
         {
            VirtualObject& right = mStack.popObject();
            VirtualObject& left  = mStack.popObject();

            mStack.pushBool(&left == &right);
         }
         break;
      case VirtualInstruction::eCmpNeqObj:
         {
            VirtualObject& right = mStack.popObject();
            VirtualObject& left  = mStack.popObject();

            mStack.pushBool(&left != &right);
         }
         break;
      case VirtualInstruction::eCmpEqAr:
         {
            VirtualArrayReference right = mStack.popArray();
            VirtualArrayReference left  = mStack.popArray();

            mStack.pushBool(left.ptr() == right.ptr());
         }
         break;
      case VirtualInstruction::eCmpNeqAr:
         {
            VirtualArrayReference right = mStack.popArray();
            VirtualArrayReference left  = mStack.popArray();

            mStack.pushBool(left.ptr() != right.ptr());
         }
         break;

      // nullekes

      case VirtualInstruction::eIsNull:
         {
            bool empty = mStack.back().isEmpty(); mStack.pop();

            mStack.pushBool(empty);
         }
         break;

      // jumps

      case VirtualInstruction::eLookup:
         {
            std::string classname = mContext.mLiteralTable[mStack.popInt()].getValue().asString();
            const VirtualLookupTable& table = mContext.mClassTable.resolve(classname).getLookupTable(instruction.getArgument());

            int codeindex = table.lookup(mStack.back());
            mStack.pop();

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
            if ( !mStack.popBool() )
            {
               mCall.jump(instruction.getArgument());
            }
         }
         break;
      case VirtualInstruction::eJumpTrue:
         {
            if ( mStack.popBool() )
            {
               mCall.jump(instruction.getArgument());
            }
         }
         break;

      // stack

      case VirtualInstruction::ePush:
         mStack.pushInt(instruction.getArgument());
         break;
      case VirtualInstruction::ePushTrue:
         mStack.pushBool(true);
         break;
      case VirtualInstruction::ePushFalse:
         mStack.pushBool(false);
         break;
      case VirtualInstruction::ePushThis:
         {
            Variant value = mStack[mCall.mStackBase];
            mStack.push(value);
         }
         break;
      case VirtualInstruction::ePushNull:
         mStack.push(Variant());
         break;
      case VirtualInstruction::eInt0:
         mStack.pushInt(0);
         break;
      case VirtualInstruction::eInt1:
         mStack.pushInt(1);
         break;
      case VirtualInstruction::eInt2:
         mStack.pushInt(2);
         break;
      case VirtualInstruction::eReal0:
         mStack.pushReal(0.0);
         break;
      case VirtualInstruction::eReal1:
         mStack.pushReal(1.0);
         break;
      case VirtualInstruction::eReal2:
         mStack.pushReal(2.0);
         break;
      case VirtualInstruction::eDup:
         mStack.push(mStack.back());
         break;
      case VirtualInstruction::eNot:
         mStack.back().setBool(!mStack.back().asBool());
         break;

      case VirtualInstruction::ePop:
         mStack.pop(instruction.getArgument()); //shrinkStack(mStack.size() - instruction.getArgument());
         break;

      case VirtualInstruction::eInstanceOf:
         {
            const std::string& name = mContext.mLiteralTable[instruction.getArgument()].getValue().asString();
            const VirtualClass* pcompareclass = mContext.mClassTable.find(name);

            ASSERT(mStack.back().isObject());
            VirtualObject& object = mStack.popObject();

            if ( object.getClass().isBaseClass(*pcompareclass)
              || object.getClass().implements(*pcompareclass) )
            {
               mStack.pushBool(true);
            }
            else
            {
               mStack.pushBool(false);
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

            VirtualObject& exception = mStack.popObject();

            throw new VirtualException(exception);
         }
         break;

      // variables

      case VirtualInstruction::eLoad:
         {
            Variant obj = mStack.back();
            mStack.pop();

            if ( obj.isObject() )
               mStack.push(obj.asObject().getMember(instruction.getArgument()));
            else if ( obj.isArray() )
               mStack.push(Variant(obj.asArray().ptr()->size()));
            else if ( obj.isEmpty() )
            {
               // error!!
               throwException("system.NullPointerException");
            }
         }
         break;
      case VirtualInstruction::eStore:
         {
            ASSERT(mStack.back().isObject());
            VirtualObject& object = mStack.popObject();

            Variant value = mStack.back();
            mStack.pop();

            object.setMember(instruction.getArgument(), value);
         }
         break;
      case VirtualInstruction::eLoadLocal:
         {
            Variant& value = mStack[mCall.mStackBase + instruction.getArgument()];

            mStack.push(value);
         }
         break;
      case VirtualInstruction::eStoreLocal:
         {
            Variant value = mStack.pop();
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
               int i = mStack.popInt();
               parray = parray->at(i).asArray().ptr();
            }

            int i = mStack.popInt();

            mStack.pop(); // pop the array from the stack

            mStack.push(parray->at(i));
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
               int i = mStack.popInt();
               parray = parray->at(i).asArray().ptr();
            }

            int i = mStack.popInt();

            mStack.pop(); // <-- pop array

            parray->at(i) = mStack.back();
            mStack.pop();
         }
         break;
      case VirtualInstruction::eStoreStatic:
         {
            int classlit = mStack.popInt();

            std::string classname = mContext.mLiteralTable[classlit].getValue().asString();
            VirtualClass& c = mContext.mClassTable.resolve(classname);

            c.setStatic(instruction.getArgument(), mStack.back());
            mStack.pop();
         }
         break;
      case VirtualInstruction::eLoadStatic:
         {
            int classlit = mStack.popInt();

            std::string classname = mContext.mLiteralTable[classlit].getValue().asString();
            const VirtualClass& c = mContext.mClassTable.resolve(classname);

            mStack.push(c.getStatic(instruction.getArgument()));
         }
         break;
      case VirtualInstruction::eLoadLiteral:
         {
            const Literal& literal = mContext.mLiteralTable[instruction.getArgument()];
            mStack.push(literal.getValue());
         }
         break;
      case VirtualInstruction::eLoadClass:
         {
            std::string name;
            if ( instruction.getArgument() == 1 )
            {
               name = mStack.back().asObject().getClass().getName();
            }
            else
            {
               name = mContext.mLiteralTable[mStack.back().asInt()].getValue().asString();
            }
            mStack.pop();

            const VirtualClass& classloader = mContext.mClassTable.resolve("system.ClassLoader");
            const VirtualFunctionTableEntry* pentry = classloader.getVirtualFunctionTable().findByName("findClass");
            const Variant& classloaderobject = classloader.getStatic(0);

            mStack.push(classloaderobject);
            mStack.pushString(name);

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
   VirtualObject* pexception = instantiate(exceptionname, -1);

   if ( reason.length() > 0 )
   {
      mStack.pushString(reason);
      execute(*pexception, "setCause");
   }

   throw new VirtualException(*pexception);
}

bool VirtualMachine::handleException(VirtualException* pexception)
{
   if ( mCall.mGuards.size() > 0 )
   {
      VirtualCall::VirtualGuard& guard = mCall.mGuards.back();
      if ( mCall.mInstructionPointer <= guard.mJumpTo )
      {
         mStack.pushObject(pexception->getException());

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

void VirtualMachine::displayException(VirtualException& exception)
{
   VirtualObject& exceptionobject = exception.getException();

   execute(exceptionobject, "getCause");
   execute(exceptionobject, "getCallStack");

   std::string callstack = mStack.popString();
   std::string cause = mStack.popString();

   std::cout << cause << std::endl << callstack;
}

// - Object creation

VirtualObject* VirtualMachine::instantiate(const std::string& classname, int constructor, void* pnativeobject)
{
   VirtualClass* pclass = doLoadClass(classname);
   if ( pclass == NULL || !pclass->canInstantiate() )
   {
      return NULL;
   }

   AutoPtr<VirtualObject> object = pclass->instantiate();
   object->setNativeObject(pnativeobject);

   Variant objectvariant(*object);

   {
      // run field initialization expressions
      const VirtualFunctionTableEntry& entry = pclass->getVirtualFunctionTable()[1];
      mStack.push(objectvariant);
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
         mStack.insert(mStack.size() - (pentry->mArguments - 1), objectvariant);
         execute(*pclass, *pentry);
      }
   }

   mObjects.push_back(object.release());
   return object.getPointer();
}

VirtualObject* VirtualMachine::instantiateNative(const std::string& classname, void* pobject, bool owned)
{
   VirtualObject* presult = NULL;
   if ( pobject != NULL )
   {
      NativeObjectMap::iterator it = mNativeObjects.find(pobject);
      if ( it != mNativeObjects.end() )
      {
         // already constructed this object earlier
         ASSERT(it->second->getNativeObject() == pobject);
         presult = it->second;
         presult->setOwner(owned);
      }
      else
      {
         // construct new instance & remember it
         presult = instantiate(classname, -1, pobject);
         ASSERT_PTR(presult); 
         ASSERT(presult->hasNativeObject() && presult->getNativeObject() == pobject);

         presult->setOwner(owned);
         mNativeObjects[pobject] = presult;
      }
   }
   
   return presult;
}

VirtualObject& VirtualMachine::instantiateArrayException(const VirtualArrayException& e)
{
   VirtualObject* presult;

   switch ( e.getKind() )
   {
      case VirtualArrayException::eOutOfBounds: 
         {
            presult = instantiate("system.ArrayIndexOutOfBoundsException", -1);
            break;
         }
      default:
         UNREACHABLE("Invalid enum value");
   }
   
   ASSERT_PTR(presult);
   return *presult;
}

VirtualObject* VirtualMachine::lookupNative(void* pobject)
{
   NativeObjectMap::iterator it = mNativeObjects.find(pobject);
   if ( it != mNativeObjects.end() )
   {
      ASSERT(it->second->getNativeObject() == pobject);
      return it->second;
   }
   return NULL;
}

VirtualArrayReference VirtualMachine::instantiateArray()
{
   ASSERT_PTR(mpArrayClass);
   VirtualArrayReference ref(mpArrayClass->instantiateArray());

   return ref;
}

// - Native interface

void VirtualMachine::registerNative(VirtualObject& object, void* pnative)
{
   ASSERT(!object.hasNativeObject());
   object.setNativeObject(pnative);

   std::pair<NativeObjectMap::iterator,bool> ret = mNativeObjects.insert(std::pair<void*, VirtualObject*>(pnative, &object));
   ASSERT(ret.second);
}

void VirtualMachine::unregisterNative(VirtualObject& object)
{
   // to be decided what to do with this.. called from the GC??

      ASSERT(object.hasNativeObject());

      // remove the object from the map
      NativeObjectMap::iterator it = mNativeObjects.find(object.getNativeObject());
      ASSERT(it != mNativeObjects.end());
      mNativeObjects.erase(it);

      if ( object.isOwner() )
      {
         const std::string& classname = object.getClass().getNativeClassName();
         if ( classname.empty() )
         {
            int aap = 5;
         }

         std::string fnc = classname + "_destruct";

         mStack.pushObject(object);
         mStack.pushInt(1);

         VirtualStackAccessor accessor(mStack);
         (*mNatives[fnc])(*this, accessor);

         mStack.pop(2);
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
      VirtualObject& object = aclass.getClassObject();

      // resolve the virtual classes
      object.setClass(mContext.mClassTable.resolve("system.Class"));

      VirtualArrayReference arrayref = object.getMember(1).asArray();
      const VirtualClass& funcclass = mContext.mClassTable.resolve("system.Function");
      for ( int index = 0; index < arrayref->size(); index++ )
      {
         VirtualObject& func = (*arrayref)[index].asObject();
         func.setClass(funcclass);
      }

      // notify the ClassLoader to add this class
      const VirtualClass& classloader = mContext.mClassTable.resolve("system.ClassLoader");
      const VirtualFunctionTableEntry& entry = classloader.getVirtualFunctionTable()[4];
      const Variant& classloaderobject = classloader.getStatic(0);

      mStack.push(classloaderobject);
      mStack.pushObject(object);

      execute(classloader, entry);
   }
}

// - Stack operations

void VirtualMachine::shrinkStack(int newsize)
{
   int diff = mStack.size() - newsize;
   ASSERT(diff >= 0);
   mStack.pop(diff);
}
