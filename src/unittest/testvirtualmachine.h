
#ifndef TEST_VIRTUALMACHINE_H
#define TEST_VIRTUALMACHINE_H

#include <cxxtest/TestSuite.h>

#include "core/vfs/filesystem.h"

#include "script/vm/virtualmachine.h"
#include "script/vm/virtualstackaccessor.h"
#include "script/vm/virtualobject.h"
#include "script/common/classregistration.h"
#include "script/common/functionregistration.h"

class NativeClass
{
public:
   int    getIntValue() { return 5; }
   String getStringValue() { return "test"; }

   int add(int a, int b) { return a + b; }
   int sub(int a, int b) { return a - b; }
   int mul(int a, int b) { return a * b; }
};

void NativeClass_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   NativeClass* pnative = new NativeClass();
   machine.registerNative(thisobject, pnative);
   thisobject.setOwner(true);
}

void NativeClass_getIntValue(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = static_cast<NativeClass*>(accessor.getThis().getNativeObject());

   accessor.setResult(pnative->getIntValue());
}

void NativeClass_getStringValue(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = static_cast<NativeClass*>(accessor.getThis().getNativeObject());

   accessor.setResult(pnative->getStringValue());
}

void NativeClass_add(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = static_cast<NativeClass*>(accessor.getThis().getNativeObject());

   int a = accessor.getInt(1);
   int b = accessor.getInt(2);

   accessor.setResult(pnative->add(a, b));
}

void NativeClass_sub(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = static_cast<NativeClass*>(accessor.getThis().getNativeObject());

   int a = accessor.getInt(1);
   int b = accessor.getInt(2);

   accessor.setResult(pnative->sub(a, b));
}

void NativeClass_mul(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = static_cast<NativeClass*>(accessor.getThis().getNativeObject());

   int a = accessor.getInt(1);
   int b = accessor.getInt(2);

   accessor.setResult(pnative->mul(a, b));
}

class TestVirtualMachine : public CxxTest::TestSuite
{
public:

   void testRun()
   {
      ClassRegistry registry;
      ClassRegistration& classreg = registry.addClass("NativeClass");
      classreg.addFunction(FunctionRegistration::Constructor("init", NativeClass_init));
      classreg.addFunction(FunctionRegistration::Function("getIntValue", NativeClass_getIntValue));
      classreg.addFunction(FunctionRegistration::Function("getStringValue", NativeClass_getStringValue));
      classreg.addFunction(FunctionRegistration::Function("add", NativeClass_add));
      classreg.addFunction(FunctionRegistration::Function("sub", NativeClass_sub));
      classreg.addFunction(FunctionRegistration::Function("mul", NativeClass_mul));

      FileSystem& fs = FileSystem::getInstance();
      fs.removeAll();
      fs.addPath("../scripts");
      fs.addPath("../src/unittest/tests");

      VirtualContext vc;
      VirtualMachine vm(vc);
      vm.mergeClassRegistry(registry);
      vm.initialize();

      TS_ASSERT(vm.execute("Test", "run"));
   }

};

#endif // TEST_VIRTUALMACHINE_H
