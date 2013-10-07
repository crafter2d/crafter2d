
#ifndef TEST_COMPILER_H
#define TEST_COMPILER_H

#include <cxxtest/TestSuite.h>

#include "core/vfs/filesystem.h"

#include "script/compiler/compiler.h"
#include "script/compiler/compilecallback.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/vminterface.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/stackcpu/stackcpu.h"

class TestCompiler : public CxxTest::TestSuite
{
   class TestCompilerCallback : public CompileCallback
   {
      TestCompiler& mTest;
   public:
      TestCompilerCallback(TestCompiler& test): mTest(test)
      {
      }

      virtual void notify(VirtualClass* pclass)
      {
         mTest.loaded();
      }
   };

public:

   TestCompiler():
      CxxTest::TestSuite(),
      mContext(),
      mVM(mContext),
      mCpu(mVM)
   {
   }
   
   void testCompilation()
   {
      FileSystem& fs = FileSystem::getInstance();
      fs.removeAll();
      fs.addPath(UTEXT("../scripts"));
      fs.addPath(UTEXT("../src/unittest/tests"));

      ClassRegistry registry;
      VMInterface::registerCommonFunctions(registry);

      Compiler compiler;
      compiler.setCPU(mCpu);
      compiler.setClassRegistry(registry);

      // compiler required classes
      TS_ASSERT(compiler.compile(UTEXT("system.Object")));
      TS_ASSERT(compiler.compile(UTEXT("system.InternalArray")));
      TS_ASSERT(compiler.compile(UTEXT("system.InternalString")));
      TS_ASSERT(compiler.compile(UTEXT("system.ClassLoader")));
      TS_ASSERT(compiler.compile(UTEXT("system.System")));
   }

   void testCallback()
   {
      TestCompilerCallback callback(*this);

      mLoaded = false;
      
      Compiler compiler;
      compiler.setCPU(mCpu);
      compiler.setCallback(callback);

      TS_ASSERT(compiler.compile(UTEXT("system.Object")));

      TS_ASSERT(mLoaded);
   }

   void loaded()
   {
      mLoaded = true;
   }

private:

   VirtualContext mContext;
   VirtualMachine mVM;
   StackCPU mCpu;
   bool     mLoaded;
};

#endif // TEST_COMPILER_H
