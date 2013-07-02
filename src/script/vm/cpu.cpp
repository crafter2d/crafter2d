
#include "cpu.h"

#include <iostream>

#include "core/smartptr/autoptr.h"
#include "core/smartptr/scopedvalue.h"
#include "core/defines.h"

#include "script/bytecode/program.h"
#include "script/vm/virtualarray.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualexception.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/virtualstring.h"

CPU::CPU(VirtualMachine& vm):
   mVM(vm),
   mGC(),
   mpProgram(NULL),
   mpArrayClass(NULL),
   mpStringClass(NULL),
   mObjectCache(),
   mBlockGC(false)
{
   mpProgram = new ByteCode::Program();
}

CPU::~CPU()
{
   // clear the garbage collector
   mGC.gc(mVM);
}

ByteCode::Program& CPU::getProgram()
{
   return *mpProgram;
}

// - Query

VirtualMachine& CPU::getVM()
{
   return mVM;
}

GarbageCollector& CPU::getGC()
{
   return mGC;
}

VirtualClass& CPU::getArrayClass()
{
   ASSERT_PTR(mpArrayClass);
   return *mpArrayClass;
}

VirtualClass& CPU::getStringClass()
{
   ASSERT_PTR(mpStringClass);
   return *mpStringClass;
}

ByteCode::IRGenerator* CPU::createIRGenerator()
{
   PURE_VIRTUAL;
   return NULL;
}

bool CPU::isGarbageCollectionBlocked() const
{
   return mBlockGC == true;
}

// - Execution

Variant CPU::execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry)
{
   PURE_VIRTUAL;
   return Variant();
}

// - Operations

void CPU::initialize(VirtualContext& context)
{
   mpArrayClass = &context.mClassTable.resolve(UTEXT("system.InternalArray"));
   mpStringClass = &context.mClassTable.resolve(UTEXT("system.InternalString"));
}

VirtualObject* CPU::instantiate(VirtualContext& context, const VirtualClass& klass, int constructor)
{
   ASSERT(klass.canInstantiate());

   VirtualObject* pobject = mObjectCache.alloc();
   klass.instantiate(*pobject);

   ScopedValue<bool> scope(&mBlockGC, true, mBlockGC);
   
   Variant objectvariant(*pobject);

   // run field initialization expressions
   const VirtualFunctionTableEntry* pentry = &klass.getVirtualFunctionTable()[1];
   execute(context, *pobject, *pentry);

   // run the constructor (entry)
   pentry = (constructor == -1) ? klass.getDefaultConstructor() : &klass.getVirtualFunctionTable()[constructor];
   execute(context, *pobject, *pentry);

   // register object with GC
   getGC().collect(pobject);

   return pobject;
}

VirtualArray* CPU::instantiateArray()
{
   ASSERT_PTR(mpArrayClass);
   VirtualArray* parray = new VirtualArray();
   mGC.collect(parray);
   return parray;
}

void CPU::release(VirtualObject& object)
{
   mObjectCache.free(&object);
}

// - Exception handling

void CPU::throwException(VirtualContext& context, const String& exceptionname, const String& reason)
{
   VirtualClass& klass = context.mClassTable.resolve(exceptionname);
   AutoPtr<VirtualObject> exception = instantiate(context, klass, -1);

   if ( reason.length() > 0 )
   {
      exception->getMember(0).setString(context.mStringCache.lookup(reason));
   }

   if ( !handleException(context, *exception) )
   {
      // oops!!
   }
   else
   {
      exception.release();
   }
}

bool CPU::handleException(VirtualContext& context, VirtualObject& exception)
{
   PURE_VIRTUAL;
   return false;
}

String CPU::buildCallStack() const
{
   PURE_VIRTUAL;
   return String();
}

void CPU::displayException(const VirtualException& exception)
{
   const VirtualObject& exceptionobject = exception.getException();

   const String& cause = exceptionobject.getMember(0).asString().getString();
   const String& callstack = exceptionobject.getMember(1).asString().getString();

   String result = UTEXT("Caught exception ")
                 + exceptionobject.getClass().getName() + UTEXT("\n")
                 + cause + UTEXT("\n")
                 + callstack;

   std::cout << cause.toUtf8() << std::endl << callstack.toUtf8();
}
