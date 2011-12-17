
#ifndef TEST_COMPILER_H
#define TEST_COMPILER_H

#include <cxxtest/TestSuite.h>

#include "core/vfs/filesystem.h"

#include "script/compiler/compiler.h"

class TestCompiler : public CxxTest::TestSuite
{
public:

   void testCompilation()
   {
      Compiler compiler;

      FileSystem& fs = FileSystem::getInstance();
      fs.removeAll();
      fs.addPath("../ascripts");
      fs.addPath("../src/unittest/tests");
      
      // compiler required classes
      TS_ASSERT(compiler.compile("System.Object"));
      TS_ASSERT(compiler.compile("System.System"));
      TS_ASSERT(compiler.compile("System.InternalArray"));
      TS_ASSERT(compiler.compile("System.ClassLoader"));
      
      // do the actual test
      TS_ASSERT(compiler.compile("Test"));
   }
};

#endif // TEST_COMPILER_H