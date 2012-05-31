
#ifndef TEST_COMMANDLINE_H
#define TEST_COMMANDLINE_H

#include <cxxtest/TestSuite.h>

#include "core/containers/list.h"
#include "core/commandline/commandline.h"

class TestCommandLine : public CxxTest::TestSuite
{
   CommandLine mCmdLine;

public:
   TestCommandLine();

   void testSize();
   void testSingleCommand();
   void testSpaceSeparatedArgument();
   void testFindValidArgument();
   void testFindNonExistingArgument();
   void testResolveCommand();
   void testResolveInvalidCommand();
};

#endif // TEST_COMMANDLINE_H
