
#ifndef TEST_VIRTUALMACHINE_H
#define TEST_VIRTUALMACHINE_H

#include <cxxtest/TestSuite.h>

#include "core/vfs/filesystem.h"

#include "script/vm/virtualmachine.h"

class TestVirtualMachine : public CxxTest::TestSuite
{
public:

   void testRun()
   {
      FileSystem& fs = FileSystem::getInstance();
      fs.removeAll();
      fs.addPath("../ascripts");
      fs.addPath("../src/unittest/tests");

      VirtualContext vc;
      VirtualMachine vm(vc);
      vm.initialize();

      TS_ASSERT(vm.execute("Test", "run"));
   }

};

#endif // TEST_VIRTUALMACHINE_H
