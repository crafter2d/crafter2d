
#ifndef TEST_COMPILER_H
#define TEST_COMPILER_H

#include <cxxtest/TestSuite.h>

#include "core/vfs/filesystem.h"

#include "script/compiler/compiler.h"
#include "script/compiler/compilecallback.h"

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

   void testCompilation()
   {
      Compiler compiler;

      FileSystem& fs = FileSystem::getInstance();
      fs.removeAll();
      fs.addPath("../scripts");
      fs.addPath("../src/unittest/tests");
      
      // compiler required classes
      TS_ASSERT(compiler.compile("System.Object"));
      TS_ASSERT(compiler.compile("System.System"));
      TS_ASSERT(compiler.compile("System.InternalArray"));
      TS_ASSERT(compiler.compile("System.ClassLoader"));
      
      // do the actual test
      TS_ASSERT(compiler.compile("Test"));
   }

   void testCallback()
   {
      TestCompilerCallback callback(*this);

      mLoaded = false;

      Compiler compiler;
      compiler.setCallback(callback);
      compiler.compile("System.Object");

      TS_ASSERT(mLoaded);
   }

   void loaded()
   {
      mLoaded = true;
   }

private:

   bool mLoaded;
};

#endif // TEST_COMPILER_H