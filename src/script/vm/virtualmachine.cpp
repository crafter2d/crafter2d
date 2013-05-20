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
//#include <vld.h>
#include <iostream>

#include "core/defines.h"
#include "core/smartptr/autoptr.h"
#include "core/string/char.h"
#include "core/conv/numberconverter.h"

#include "script/compiler/compiler.h"
#include "script/common/literal.h"
#include "script/bytecode/program.h"

#include "stackcpu/stackcpu.h"

#include "virtualarray.h"
#include "virtualarrayexception.h"
#include "virtualfunctionnotfoundexception.h"
#include "virtualinstructiontable.h"
#include "virtualcontext.h"
#include "virtualobject.h"
#include "virtualclass.h"
#include "virtualexception.h"
#include "virtualfunctiontable.h"
#include "virtualfunctiontableentry.h"
#include "virtuallookuptable.h"
#include "virtualstackaccessor.h"

VirtualMachine::VirtualMachine(VirtualContext& context):
   mContext(context),
   mCallback(*this),
   mCompiler(),
   mRootObjects(),
   mNativeObjects(),
   mState(eInit),
   mpArrayClass(NULL),
   mpStringClass(NULL),
   mpCPU(NULL),
   mLoaded(false)
{
   mpCPU = new StackCPU(*this);

   mCompiler.setCPU(*mpCPU);
   mCompiler.setCallback(mCallback);
}

VirtualMachine::~VirtualMachine()
{
   // set destruct state, native object notifies vm when it is destructed
   // resulting in double delete.
   mState = eDestruct;
   mNativeObjects.clear();
   mCompiler.cleanUp();
}

// - Initialization

void VirtualMachine::initialize()
{
   ClassRegistry registry;
   VMInterface::registerCommonFunctions(registry);
   mergeClassRegistry(registry);
   mContext.mNativeRegistry.add(mCompiler.getClassRegistry());

   // preload some common classes
   loadClass("system.Object");
   loadClass("system.InternalArray");
   loadClass("system.InternalString");

   // resolve the internal classes
   mpArrayClass = &mContext.mClassTable.resolve("system.InternalArray");
   mpStringClass = &mContext.mClassTable.resolve("system.InternalString");
   mpCPU->initialize(mContext);

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

bool VirtualMachine::loadClass(const String& classname)
{
   return doLoadClass(classname) != NULL;
}

void VirtualMachine::mergeClassRegistry(const ClassRegistry& registry)
{
   mCompiler.setClassRegistry(registry);
}

// - Stack access

void VirtualMachine::addRootObject(VirtualObject& object)
{
   mRootObjects.push_back(&object);
}

// - Execution

bool VirtualMachine::execute(const String& classname, const String& function)
{
   const VirtualClass* pclass = doLoadClass(classname);
   if ( pclass == NULL )
      return false;

   const VirtualFunctionTableEntry* pentry = pclass->getVirtualFunctionTable().findByName(function);
   if ( pentry == NULL )
      return false;

   mpCPU->execute(mContext, *pclass, *pentry);

   return true;
}

Variant VirtualMachine::execute(VirtualObject& object, const String& function, int argc, Variant* pargs)
{
   const VirtualClass& vclass = object.getClass();
   const VirtualFunctionTableEntry* pentry = vclass.getVirtualFunctionTable().findByName(function);
   if ( pentry == NULL )
   {
      throw new VirtualFunctionNotFoundException(object.getClass().getName(), function);
   }

   return mpCPU->execute(mContext, object, *pentry, argc, pargs);
}

void VirtualMachine::execute(const VirtualClass& vclass, const VirtualFunctionTableEntry& entry)
{
   /*
   mCallStack.push(mCall);
   mCall.start(vclass, entry, mStack.size());

#ifdef _DEBUG
   int len;
   const char* pclass = vclass.getName().toUtf8(len);
   const char* pentry = entry.mName.toUtf8(len);
#endif

   const VirtualInstructionTable& instructions = mContext.mInstructions;

   while ( mState != eReturn )
   {
      try
      {
         const VirtualInstruction& inst = instructions[mCall.mInstructionPointer++];

         execute(vclass, inst);
      }
      catch ( VirtualFunctionNotFoundException* )
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

#ifdef _DEBUG
   delete[] pclass;
   delete[] pentry;
#endif

   mCall = mCallStack.top();
   mCallStack.pop();

   mState = eRunning;
   */
}

void VirtualMachine::execute(const VirtualClass& vclass, const VirtualInstruction& instruction)
{
   /*
   switch ( instruction.getOpcode() )
   {
      case VirtualInstruction::eReserve:
         {
            // reserve 'argument' number of places on stack for arguments/variables
            mStack.push(instruction.getArgument());
         }
         break;
      case VirtualInstruction::eNew:
         {
            const String& type = mContext.mLiteralTable[mStack.popInt()].getValue().asString().getString();

            VirtualObject* pobject = instantiate(type, instruction.getArgument());
            ASSERT_PTR(pobject);

            mStack.pushObject(*pobject);
         }
         break;
      case VirtualInstruction::eNewNative:
         {            
            int arguments = mStack.popInt();

            const Variant& object = mStack[mStack.size() - arguments];
            ASSERT(object.isObject());

            if ( !object.asObject().hasNativeObject() )
            {
               VirtualStackAccessor accessor(mContext, mStack, arguments);
               (*mCompiler.getClassRegistry().getCallback(instruction.getArgument()))(*this, accessor);
            }
         }
         break;
      case VirtualInstruction::eNewArray:
         {
            // n sizes
            // NewArray dimension

            int arraydimension = instruction.getArgument();

            // no full support yet for multi-dimensional arrays
            VirtualArray* parray = instantiateArray();
            VirtualArray* pinit = parray;
            for ( int index = 0; index < arraydimension; index++ )
            {
               int size = mStack.popInt();

               pinit->addLevel(size);
               if ( index < arraydimension - 1 )
               { // need to implement this for multi dimensional arrays
                  // VirtualArray* ptemp = pinit;
               }
            }

            mStack.pushArray(*parray);
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
               throwException("system.NullPointerException", "");
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
            int arguments = mStack.popInt();

            VirtualStackAccessor accessor(mContext, mStack, arguments);
            (*mCompiler.getClassRegistry().getCallback(instruction.getArgument()))(*this, accessor);

            mStack.pop(arguments); // pop the arguments
            if ( accessor.hasResult() )
            {
               mStack.push(accessor.getResult());
            }
         }
         break;
      case VirtualInstruction::eCallStatic:
         {
            int classlit = mStack.popInt();

            String classname = mContext.mLiteralTable[classlit].getValue().asString().getString();

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
            String result;
            NumberConverter::getInstance().format(result, mStack.popInt());
            mStack.pushString(mContext.mStringCache.lookup(result));
         }
         break;
      case VirtualInstruction::eReal2Int:
         {
            mStack.back().real2int();
         }
         break;
      case VirtualInstruction::eReal2String:
         {
            String result;
            NumberConverter::getInstance().format(result, mStack.popReal());
            mStack.pushString(mContext.mStringCache.lookup(result));
         }
         break;
      case VirtualInstruction::eChar2String:
         {
            String result;
            result += mStack.popChar();
            mStack.pushString(mContext.mStringCache.lookup(result));
         }
         break;
      case VirtualInstruction::eBoolean2String:
         {
            bool value = mStack.popBool();
            String string(value ? String("true") : String("false"));
            mStack.pushString(mContext.mStringCache.lookup(string));
         }
         break;
      case VirtualInstruction::eString2Int:
         {
            const String& value = mStack.popString();
            int result = NumberConverter::getInstance().toInt(value);
            mStack.pushInt(result);            
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
               throwException("system.DivideByZeroException", "");
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
            String left = mStack.popString();

            mStack.pushString(mContext.mStringCache.lookup(left + right));
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
            String right = mStack.popString();
            String left = mStack.popString();

            mStack.pushString(mContext.mStringCache.lookup(left + right));
         }
         break;
      case VirtualInstruction::eCmpEqStr:
         {
            String right = mStack.popString();
            String left = mStack.popString();

            mStack.pushBool(left == right);
         }
         break;
      case VirtualInstruction::eCmpNeqStr:
         {
            String right = mStack.popString();
            String left = mStack.popString();

            mStack.pushBool(!(left == right));
         }
         break;
      case VirtualInstruction::eCmpLeStr:
         {
            String right = mStack.popString();
            String left = mStack.popString();

            mStack.pushBool(left <= right);
         }
         break;
      case VirtualInstruction::eCmpLtStr:
         {
            String right = mStack.popString();
            String left = mStack.popString();

            mStack.pushBool(left < right);
         }
         break;
      case VirtualInstruction::eCmpGeStr:
         {
            String right = mStack.popString();
            String left = mStack.popString();

            mStack.pushBool(left > right);
         }
         break;
      case VirtualInstruction::eCmpGtStr:
         {
            String right = mStack.popString();
            String left = mStack.popString();

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
            VirtualArray& right = mStack.popArray();
            VirtualArray& left  = mStack.popArray();

            mStack.pushBool(&left == &right);
         }
         break;
      case VirtualInstruction::eCmpNeqAr:
         {
            VirtualArray& right = mStack.popArray();
            VirtualArray& left  = mStack.popArray();

            mStack.pushBool(&left != &right);
         }
         break;

      // nullekes

      case VirtualInstruction::eIsNull:
         {
            bool empty = mStack.back().isEmpty(); 
            mStack.pop(1);

            mStack.pushBool(empty);
         }
         break;

      // jumps

      case VirtualInstruction::eLookup:
         {
            String classname = mContext.mLiteralTable[mStack.popInt()].getValue().asString().getString();
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
         mStack.pop(instruction.getArgument());
         break;

      case VirtualInstruction::eInstanceOf:
         {
            const String& name = mContext.mLiteralTable[instruction.getArgument()].getValue().asString().getString();
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
            Variant obj = mStack.pop();

            if ( obj.isObject() )
               mStack.push(obj.asObject().getMember(instruction.getArgument()));
            else if ( obj.isArray() )
               mStack.push(Variant(obj.asArray().size())); // length attribute
            else if ( obj.isEmpty() )
            {
               throwException("system.NullPointerException", "");
            }
         }
         break;
      case VirtualInstruction::eStore:
         {
            ASSERT(mStack.back().isObject());
            VirtualObject& object = mStack.popObject();

            object.setMember(instruction.getArgument(), mStack.pop());
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
            mStack[mCall.mStackBase + instruction.getArgument()] = mStack.pop();
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

            VirtualArray* parray = &variant.asArray();
            for ( int index = 0; index < instruction.getArgument() - 1; index++ )
            {
               int i = mStack.popInt();
               parray = &parray->at(i).asArray();
            }

            int i = mStack.popInt();

            mStack.pop(1); // pop the array from the stack

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

            VirtualArray* parray = &variant.asArray();
            for ( int index = 0; index < instruction.getArgument()-1; index++ )
            {
               int i = mStack.popInt();
               parray = &parray->at(i).asArray();
            }

            int i = mStack.popInt();

            mStack.pop(1); // <-- pop array

            parray->at(i) = mStack.pop();
         }
         break;
      case VirtualInstruction::eStoreStatic:
         {
            int classlit = mStack.popInt();

            const String& classname = mContext.mLiteralTable[classlit].getValue().asString().getString();
            VirtualClass& c = mContext.mClassTable.resolve(classname);

            c.setStatic(instruction.getArgument(), mStack.pop());
         }
         break;
      case VirtualInstruction::eLoadStatic:
         {
            int classlit = mStack.popInt();

            const String& classname = mContext.mLiteralTable[classlit].getValue().asString().getString();
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
            String name;
            if ( instruction.getArgument() == 1 )
            {
               name = mStack.back().asObject().getClass().getName();
            }
            else
            {
               name = mContext.mLiteralTable[mStack.back().asInt()].getValue().asString().getString();
            }
            mStack.pop(1);

            const VirtualClass& classloader = mContext.mClassTable.resolve("system.ClassLoader");
            const VirtualFunctionTableEntry* pentry = classloader.getVirtualFunctionTable().findByName("findClass");
            const Variant& classloaderobject = classloader.getStatic(0);

            mStack.push(classloaderobject);
            mStack.pushString(mContext.mStringCache.lookup(name));

            execute(classloader, *pentry);
         }
         break;

      case VirtualInstruction::eNop:
      default:
         break;
   }
   */
}

void VirtualMachine::mark()
{
   mpCPU->mark();

   mContext.mClassTable.mark();  // marks all statics
   for ( std::size_t index = 0; index < mRootObjects.size(); index++ )
   {
      VirtualObject* pobject = mRootObjects[index];
      pobject->mark();
   }
}

// - Exception

String VirtualMachine::buildCallStack() const
{
   return mpCPU->buildCallStack();
}

// - Object creation

VirtualObject* VirtualMachine::instantiate(const String& classname, int constructor)
{
   VirtualObject* presult = NULL;
   VirtualClass* pclass = doLoadClass(classname);
   if ( pclass != NULL )
   {
      presult = mpCPU->instantiate(mContext, *pclass, constructor);
   }
   return presult;
}

VirtualObject* VirtualMachine::instantiateNative(const String& classname, void* pobject, bool owned)
{
   ASSERT_PTR(pobject);

   VirtualObject* presult = NULL;
   NativeObjectMap::iterator it = mNativeObjects.find(pobject);
   if ( it != mNativeObjects.end() )
   {
      presult = it->second;
   }
   else
   {
      presult = instantiate(classname);
      mNativeObjects.insert(std::pair<void*, VirtualObject*>(pobject, presult));
   }
   
   presult->setNativeObject(pobject);
   presult->setOwner(owned);
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

VirtualArray* VirtualMachine::instantiateArray()
{
   ASSERT_PTR(mpArrayClass);
   VirtualArray* parray = new VirtualArray();
   mGC.collect(parray);
   return parray;
}

void VirtualMachine::release(VirtualObject& object)
{
   mObjectCache.free(&object);
}

// - Native interface

void VirtualMachine::registerNative(VirtualObject& object, void* pnative)
{
   ASSERT(!object.hasNativeObject());
   object.setNativeObject(pnative);
   object.setOwner(true);

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

   object.setNativeObject(NULL);

   if ( object.isOwner() )
   {
      const VirtualFunctionTableEntry* pentry = object.getClass().getVirtualFunctionTable().findByName("finalize");
      if ( pentry != NULL )
      {
         mpCPU->execute(mContext, object, *pentry);
      }
   }
}

// - Callbacks

VirtualClass* VirtualMachine::doLoadClass(const String& classname)
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
   mContext.mClassTable.insert(pclass);

   if ( pclass->getName() == "system.ClassLoader" )
   {
      int aap = 5;
   }

   createClass(*pclass);

   // execute the static initialization body
   VirtualFunctionTableEntry& entry = pclass->getVirtualFunctionTable()[0];
   mpCPU->execute(mContext, *pclass, entry);
}

void VirtualMachine::createClass(const VirtualClass& aclass)
{
   if ( mLoaded )
   {
      VirtualObject& object = aclass.getClassObject();

      // resolve the virtual classes
      object.setClass(mContext.mClassTable.resolve("system.Class"));

      VirtualArray& funcarray = object.getMember(1).asArray();
      const VirtualClass& funcclass = mContext.mClassTable.resolve("system.Function");
      for ( int index = 0; index < funcarray.size(); index++ )
      {
         VirtualObject& func = funcarray[index].asObject();
         func.setClass(funcclass);
      }

      // notify the ClassLoader to add this class
      const VirtualClass& classloader = mContext.mClassTable.resolve("system.ClassLoader");
      const VirtualFunctionTableEntry& entry = classloader.getVirtualFunctionTable()[4];
      const Variant& classloaderobject = classloader.getStatic(0);
      ASSERT(classloaderobject.isObject());

      Variant arg(object);
      mpCPU->execute(mContext, classloaderobject.asObject(), entry, 1, &arg);

      execute(classloader, entry);
   }
}
