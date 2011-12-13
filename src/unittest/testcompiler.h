
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

      FileSystem::getInstance().addPath("../ascripts");
      FileSystem::getInstance().addPath("../src/unittest/tests");
      
      TS_ASSERT(compiler.compile("System.Object"));
      TS_ASSERT(compiler.compile("Test"));
   }
};

#endif // TEST_COMPILER_H