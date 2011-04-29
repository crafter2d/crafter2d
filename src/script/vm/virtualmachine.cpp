
#include "virtualmachine.h"

#include <iostream>

#include "core/defines.h"

#include "script/compiler/compiler.h"
#include "script/common/literal.h"

#include "virtualinstructiontable.h"
#include "virtualcontext.h"
#include "virtualclassobject.h"
#include "virtualobjectreference.h"
#include "virtualclass.h"
#include "virtualarrayobject.h"
#include "virtualarrayreference.h"
#include "virtualexception.h"
#include "virtualfunctiontable.h"
#include "virtualfunctiontableentry.h"
#include "virtualnativeobject.h"

class World
{
public:
   World(const std::string& name): mName(name) {}

   std::string mName;
};

void Console_println(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   std::cout << accessor.getString(1);
}

void Class_doNewInstance(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference thisobject = accessor.getThis();
   VirtualObjectReference classobject = accessor.getObject(1);

   std::string name = classobject->asInstance()[0].asString();
   VirtualObjectReference object = machine.instantiate(name);

   accessor.setResult(object);
}

void Function_doInvoke(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference thisobject = accessor.getThis();
   VirtualObjectReference instance = accessor.getObject(1);

   std::string fncname = thisobject->asInstance()[0].asString();

   machine.execute(instance, fncname);
}

void World_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   World* pworld = new World("joop");

   VirtualObjectReference ref = machine.instantiateNative("World", pworld);

   accessor.setResult(ref);
}

void World_getName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   accessor.setResult(pworld->mName);
}

VirtualMachine::VirtualMachine():
   mContext(),
   mCallback(*this),
   mCompiler(),
   mStack(),
   mCallStack(),
   mCall(),
   mException(NULL),
   mNatives(),
   mState(eRunning),
   mLoaded(false)
{
   mCompiler.setCallback(mCallback);

   loadClass("Object");
   loadClass("InternalArray");
   loadClass("ClassLoader");
   loadClass("System");

   mNatives.insert(std::pair<std::string, callbackfnc>("Console_println", Console_println));
   mNatives.insert(std::pair<std::string, callbackfnc>("Class_doNewInstance", Class_doNewInstance));
   mNatives.insert(std::pair<std::string, callbackfnc>("Function_doInvoke", Function_doInvoke));
   mNatives.insert(std::pair<std::string, callbackfnc>("World_init", World_init));
   mNatives.insert(std::pair<std::string, callbackfnc>("World_getName", World_getName));

   mState = eRunning;
   mLoaded = true;
}

// - Execution

void VirtualMachine::execute(const std::string& classname, const std::string& function)
{
   VirtualClass* pclass = mContext.mClassTable.find(classname);
   if ( pclass == NULL )
   {
      pclass = loadClass(classname);
   }

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
      }
      catch (...)
      {
      }
   }
}

void VirtualMachine::execute(const VirtualObjectReference& object, const std::string function)
{
   const VirtualClass& vclass = object->getClass();
   const VirtualFunctionTableEntry* pentry = vclass.getVirtualFunctionTable().findByName(function);
   if ( pentry == NULL )
   {
      throw std::exception();
   }

   mStack.push_back(Variant(object));

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

         execute(inst);
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

void VirtualMachine::execute(const VirtualInstruction& instruction)
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
      case VirtualInstruction::eNewNative:
         {
            int type = mStack.back().asInt();
            mStack.pop_back();

            const std::string& classname = mContext.mLiteralTable[type].getValue().asString();
            const VirtualClass& vclass = mContext.mClassTable.resolve(classname);
            const VirtualFunctionTableEntry& entry = vclass.getVirtualFunctionTable()[instruction.getArgument()];

            execute(vclass, entry);
         }
         break;
      case VirtualInstruction::eCallSuper:
         {
            int arguments = mStack.back().asInt();
            mStack.pop_back();

            const Variant& object = mStack[mStack.size() - arguments];
            ASSERT(object.isObject());

            const VirtualClass& theclass = object.asObject()->getClass().getBaseClass();
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
            int args = mStack.back().asInt();
            mStack.pop_back();

            VirtualStackAccessor accessor(mStack, args);
            (*mNatives[fnc])(*this, accessor);

            mStack.resize(mStack.size() - args);
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
            int right = mStack.back().asReal(); mStack.pop_back();
            int left  = mStack.back().asReal(); mStack.pop_back();

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
         mStack.push_back(Variant(!mStack.back().asBool()));

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
               mStack.push_back((*obj.asObject()).asInstance()[instruction.getArgument()]);
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

            obj.asObject()->asInstance()[instruction.getArgument()] = value;
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
            const std::string& classname = mContext.mLiteralTable[mStack.back().asInt()].getValue().asString();
            mStack.pop_back();
            
            const VirtualClass& classloader = mContext.mClassTable.resolve("ClassLoader");
            const VirtualFunctionTableEntry* pentry = classloader.getVirtualFunctionTable().findByName("findClass");
            const Variant& classloaderobject = classloader.getStatic(0);

            mStack.push_back(classloaderobject);
            mStack.push_back(Variant(classname));
      
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
   VirtualClass* pclass = mContext.mClassTable.find(classname);
   ASSERT_PTR(pclass);
            
   VirtualObjectReference object(pclass->instantiate());
   Variant objectvariant(object);

   {
      // run field initialization expressions
      const VirtualFunctionTableEntry& entry = pclass->getVirtualFunctionTable()[1];
      mStack.push_back(objectvariant);
      execute(*pclass, entry);
   }

   {
      // call the constructor as coded
      const VirtualFunctionTableEntry& entry = pclass->getVirtualFunctionTable()[constructor];
      mStack.insert(mStack.begin() + mStack.size() - (entry.mArguments - 1), objectvariant);
      execute(*pclass, entry);
   }

   return object;
}

VirtualObjectReference VirtualMachine::instantiateNative(const std::string& classname, void* pobject)
{
   VirtualObjectReference object(new VirtualNativeObject(pobject));
   object->setClass(mContext.mClassTable.resolve(classname));
   return object;
}

VirtualArrayReference VirtualMachine::instantiateArray()
{
   VirtualClass& internalarray = mContext.mClassTable.resolve("InternalArray");
   VirtualArrayReference ref(internalarray.instantiateArray());

   return ref;
}

// - Loading

VirtualClass* VirtualMachine::loadClass(const std::string& classname)
{
   if ( !mCompiler.compile(classname) )
   {
      throwException("ClassNotFoundException");
   }

   return mContext.mClassTable.find(classname);
}

void VirtualMachine::classLoaded(VirtualClass* pclass)
{
   int offset = mContext.mInstructions.size();

   mContext.mClassTable.insert(pclass);

   if ( pclass->hasBaseName() )
   {
      std::string base = pclass->getBaseName();
      pclass->setBaseClass(mContext.mClassTable.resolve("Object"));

      VirtualFunctionTable& vtable = pclass->getVirtualFunctionTable();
      vtable.merge(pclass->getBaseClass().getVirtualFunctionTable());
      vtable.offset(offset);
   }
   
   mContext.mInstructions.add(pclass->getInstructions());

   int lookback = 0;

   for ( int index = offset; index < mContext.mInstructions.size(); index++ )
   {
      VirtualInstruction& instruction = mContext.mInstructions[index];

      switch ( instruction.getInstruction() )
      {
         case VirtualInstruction::eCallStatic:
            lookback++;

         case VirtualInstruction::eNew:
         case VirtualInstruction::eNewNative:
         case VirtualInstruction::eLoadClass:
         case VirtualInstruction::eLoadStatic:
         case VirtualInstruction::eStoreStatic:
            lookback++;

         case VirtualInstruction::eCallNative:
         case VirtualInstruction::eLoadLiteral:
            {
               VirtualInstruction& previous = mContext.mInstructions[index - lookback];
               const Literal& literal = mCompiler.lookupLiteral(previous.getArgument());

               int i = mContext.mLiteralTable.indexOf(literal);
               if ( i == mContext.mLiteralTable.size() )
               {
                  i = mContext.mLiteralTable.insert(literal.clone());
               }

               previous.setArgument(i);

               lookback = 0;
            }
            break;

         default:
            break;
      }
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
      VirtualClassObject& object = objectref->asInstance();
      object.setClass(mContext.mClassTable.resolve("Class"));

      VirtualArrayReference arrayref = object[1].asArray();
      const VirtualClass& funcclass = mContext.mClassTable.resolve("Function");
      for ( int index = 0; index < arrayref->size(); index++ )
      {
         VirtualObjectReference funcref = (*arrayref)[index].asObject();
         VirtualObject& func = *funcref;
         func.setClass(funcclass);
      }

      // notify the ClassLoader to add this class
      const VirtualClass& classloader = mContext.mClassTable.resolve("ClassLoader");
      const VirtualFunctionTableEntry& entry = classloader.getVirtualFunctionTable()[4];
      const Variant& classloaderobject = classloader.getStatic(0);

      mStack.push_back(classloaderobject);
      mStack.push_back(Variant(objectref));
      
      execute(classloader, entry);
   }
}
