
#include "testcommandline.h"

#include "core/commandline/commandline.h"

static int   sArgc = 6;
static char* sArgv[] = { "unittest.exe", "doit", "argument=value", "argument2", "=", "anothervalue" };

TestCommandLine::TestCommandLine():
   mCmdLine(sArgc, sArgv)
{
}

void TestCommandLine::testSize()
{
   TS_ASSERT_EQUALS(mCmdLine.size(), 3)
}

void TestCommandLine::testSingleCommand()
{
   TS_ASSERT(mCmdLine.hasArgument(UTEXT("doit")));
}

void TestCommandLine::testSpaceSeparatedArgument()
{
   String name("argument2");
   TS_ASSERT_THROWS_NOTHING(mCmdLine.getArgument(name));

   auto pargument = mCmdLine.getArgument(name);
   TS_ASSERT(pargument != nullptr);
   TS_ASSERT(pargument->getValue() == UTEXT("anothervalue"));
}

void TestCommandLine::testFindValidArgument()
{
   const CommandLineArgument* pargument = mCmdLine.getArgument(UTEXT("argument"));
   TS_ASSERT(pargument != NULL);
}

void TestCommandLine::testFindNonExistingArgument()
{
   const CommandLineArgument* pargument = mCmdLine.getArgument(UTEXT("non-existing-argument"));
   TS_ASSERT(pargument == NULL);
}

void TestCommandLine::testResolveCommand()
{
   auto pargument = mCmdLine.getArgument(UTEXT("argument"));

   TS_ASSERT(pargument != NULL);
   TS_ASSERT(pargument->getType() == CommandLineArgument::eArgument);
   TS_ASSERT(pargument->getValue() == UTEXT("value"));
}

void TestCommandLine::testResolveInvalidCommand()
{
   //TS_ASSERT_THROWS(mCmdLine.getArgument(UTEXT("non-existing-argument")), CommandLineException*);
}
