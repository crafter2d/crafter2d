
#ifndef TEST_COMMANDLINE_H
#define TEST_COMMANDLINE_H

#include <unittest++/UnitTest++.h>

#include "core/containers/list.h"
#include "core/commandline/commandline.h"

static const int   sArgc = 6;
static const char* sArgv[] = { "unittest.exe", "doit", "argument=value", "argument2", "=", "anothervalue" };

SUITE(TestCommandLine)
{
   class Fixture
   {
   public:
      Fixture() :
         mCmdLine(sArgc, sArgv)
      {
      }

      CommandLine mCmdLine;
   };

   TEST_FIXTURE(Fixture, testSize)
   {
      CHECK_EQUAL(mCmdLine.size(), 3);
   }

   TEST_FIXTURE(Fixture, testSingleCommand)
   {
      CHECK(mCmdLine.hasArgument(UTEXT("doit")));
   }

   TEST_FIXTURE(Fixture, testSpaceSeparatedArgument)
   {
      auto parg = mCmdLine.getArgument(UTEXT("argument2"));
      CHECK(parg != nullptr);
      CHECK(parg->getValue() == UTEXT("anothervalue"));
   }

   TEST_FIXTURE(Fixture, testFindValidArgument)
   {
      const CommandLineArgument* pargument = mCmdLine.getArgument(UTEXT("argument"));
      CHECK(pargument != nullptr);
   }

   TEST_FIXTURE(Fixture, testFindNonExistingArgument)
   {
      const CommandLineArgument* pargument = mCmdLine.getArgument(UTEXT("non-existing-argument"));
      CHECK(pargument == nullptr);
   }

   TEST_FIXTURE(Fixture, testResolveCommand)
   {
      auto pargument = mCmdLine.getArgument(UTEXT("argument"));

      CHECK(pargument != nullptr);
      CHECK(pargument->getType() == CommandLineArgument::eArgument);
      CHECK(pargument->getValue() == UTEXT("value"));
   }

   TEST_FIXTURE(Fixture, testResolveInvalidCommand)
   {
      //TS_ASSERT_THROWS(mCmdLine.getArgument(UTEXT("non-existing-argument")), CommandLineException*);
   }
};

#endif // TEST_COMMANDLINE_H
