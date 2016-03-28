
#include "cpu.h"

#include <string>
#include <iostream>

#include "core/log/log.h"
#include "core/smartptr/autoptr.h"
#include "core/smartptr/scopedvalue.h"
#include "core/defines.h"

#include "mod_yas/bytecode/program.h"
#include "mod_yas/vm/virtualarray.h"
#include "mod_yas/vm/virtualclass.h"
#include "mod_yas/vm/virtualcontext.h"
#include "mod_yas/vm/virtualexception.h"
#include "mod_yas/vm/virtualobject.h"
#include "mod_yas/vm/virtualmachine.h"
#include "mod_yas/vm/virtualstring.h"

namespace ByteCode
{
   CPU::CPU(VirtualMachine& vm):
      mVM(vm),
      mGC(),
      mpArrayClass(NULL),
      mpStringClass(NULL),
      mObjectCache(),
      mBlockGC(false)
   {
   }

   CPU::~CPU()
   {
      // clear the garbage collector
      mGC.gc(mVM);
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

   bool CPU::isGarbageCollectionBlocked() const
   {
      return mBlockGC == true;
   }

   // - Operations

   void CPU::initialize(VirtualContext& context)
   {
      mpArrayClass = &context.resolveClass(UTEXT("system.InternalArray"));
      mpStringClass = &context.resolveClass(UTEXT("system.InternalString"));
   }

   VirtualObject& CPU::instantiate(VirtualContext& context, const VirtualClass& klass, int constructor)
   {
      ASSERT(klass.canInstantiate());

      VirtualObject* pobject = mObjectCache.alloc();
      klass.instantiate(*pobject);

      ScopedValue<bool> scope(&mBlockGC, true, mBlockGC);
   
      VirtualValue objectvariant(*pobject);

      // run field initialization expressions
      const VirtualFunctionTableEntry* pentry = &klass.getVirtualFunctionTable()[1];
      execute(context, *pobject, *pentry, 0, NULL);

      // run the constructor (entry)
      pentry = (constructor == -1) ? klass.getDefaultConstructor() : &klass.getVirtualFunctionTable()[constructor];
      execute(context, *pobject, *pentry);

      // register object with GC
      getGC().collect(pobject);

      return *pobject;
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
      VirtualClass& klass = context.resolveClass(exceptionname);
      VirtualObject& exception = instantiate(context, klass, -1);
      String callstack = buildCallStack();

      exception.getMember(0).setString(context.mStringCache.lookup(reason));
      exception.getMember(1).setString(context.mStringCache.lookup(callstack));

      if ( !handleException(context, exception) )
      {
         String msg = UTEXT("Unhandled exception '") + klass.getName() + UTEXT("'\n") + callstack;
         std::string m = msg.toUtf8();
         Log::getInstance().error(m.c_str());
      }
   }

   void CPU::displayException(const VirtualException& exception)
   {
      const VirtualObject& exceptionobject = exception.getException();

      const String& cause = exceptionobject.getMember(0).asString();
      const String& callstack = exceptionobject.getMember(1).asString();

      String result = UTEXT("Caught exception ")
                    + exceptionobject.getClass().getName() + UTEXT("\n")
                    + cause + UTEXT("\n")
                    + callstack;

      std::cout << cause.toUtf8() << std::endl << callstack.toUtf8();
   }
}