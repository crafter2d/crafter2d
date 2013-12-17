
#ifndef TEST_VIRTUALMACHINE_H
#define TEST_VIRTUALMACHINE_H

#include <cxxtest/TestSuite.h>

#include "core/vfs/filesystem.h"
#include "core/smartptr/autoptr.h"

#include "script/vm/virtualmachine.h"
#include "script/vm/virtualstackaccessor.h"
#include "script/vm/virtualobject.h"
#include "script/common/functionregistration.h"

class NativeClass
{
public:
   int    getIntValue() { return 5; }
   String getStringValue() { return UTEXT("test"); }

   int add(int a, int b) { return a + b; }
   int sub(int a, int b) { return a - b; }
   int mul(int a, int b) { return a * b; }
};

void NativeClass_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   NativeClass* pnative = new NativeClass();
   machine.registerNative(thisobject, pnative);
}

void NativeClass_getIntValue(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = accessor.getThis().getNativeObject<NativeClass*>();

   accessor.setResult(pnative->getIntValue());
}

void NativeClass_getStringValue(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = accessor.getThis().getNativeObject<NativeClass*>();

   accessor.setResult(pnative->getStringValue());
}

void NativeClass_add(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = accessor.getThis().getNativeObject<NativeClass*>();

   int a = accessor.getInt(1);
   int b = accessor.getInt(2);

   accessor.setResult(pnative->add(a, b));
}

void NativeClass_sub(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = accessor.getThis().getNativeObject<NativeClass*>();

   int a = accessor.getInt(1);
   int b = accessor.getInt(2);

   accessor.setResult(pnative->sub(a, b));
}

void NativeClass_mul(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   NativeClass* pnative = accessor.getThis().getNativeObject<NativeClass*>();

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
      registry.addClass(UTEXT("NativeClass"));
      registry.addFunction(UTEXT("NativeClass()"), NativeClass_init);
      registry.addFunction(UTEXT("getIntValue()"), NativeClass_getIntValue);
      registry.addFunction(UTEXT("getStringValue()"), NativeClass_getStringValue);
      registry.addFunction(UTEXT("add(int, int)"), NativeClass_add);
      registry.addFunction(UTEXT("sub(int, int)"), NativeClass_sub);
      registry.addFunction(UTEXT("mul(int, int)"), NativeClass_mul);

      FileSystem& fs = FileSystem::getInstance();
      fs.removeAll();
      fs.addPath(UTEXT("../scripts"));
      fs.addPath(UTEXT("../src/unittest/tests"));

      VirtualContext vc;
      VirtualMachine vm(vc);
      vm.mergeClassRegistry(registry);
      vm.initialize();

      AutoPtr<VirtualObject> object = vm.instantiate(UTEXT("Test"));

      TS_ASSERT(object.hasPointer());
      if ( object.hasPointer() )
      {
         TS_ASSERT(vm.execute(*object, UTEXT("run")).isEmpty());
      }
   }

};

#endif // TEST_VIRTUALMACHINE_H
