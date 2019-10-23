
#include <gtest/gtest.h>

#include "core/commandline/commandline.h"

static const int   sArgc = 6;
static const char* sArgv[] = { "unittest.exe", "doit", "argument=value", "argument2", "=", "anothervalue" };

class CommandLineFixture : public testing::Test
{
public:
   CommandLineFixture() :
      mCmdLine(sArgc, sArgv)
   {
   }

   CommandLine mCmdLine;
};

TEST_F(CommandLineFixture, testSize)
{
   EXPECT_EQ(mCmdLine.size(), 3);
}

TEST_F(CommandLineFixture, testSingleCommand)
{
   EXPECT_TRUE(mCmdLine.hasArgument(UTEXT("doit")));
}

TEST_F(CommandLineFixture, testSpaceSeparatedArgument)
{
   auto parg = mCmdLine.getArgument(UTEXT("argument2"));
   EXPECT_NE(parg, nullptr);
   EXPECT_EQ(parg->getValue(), UTEXT("anothervalue"));
}

TEST_F(CommandLineFixture, testFindValidArgument)
{
   const CommandLineArgument* pargument = mCmdLine.getArgument(UTEXT("argument"));
   EXPECT_NE(pargument, nullptr);
}

TEST_F(CommandLineFixture, testFindNonExistingArgument)
{
   const CommandLineArgument* pargument = mCmdLine.getArgument(UTEXT("non-existing-argument"));
   EXPECT_EQ(pargument, nullptr);
}

TEST_F(CommandLineFixture, testResolveCommand)
{
   auto pargument = mCmdLine.getArgument(UTEXT("argument"));

   EXPECT_NE(pargument, nullptr);
   EXPECT_EQ(pargument->getType(), CommandLineArgument::eArgument);
   EXPECT_EQ(pargument->getValue(), UTEXT("value"));
}

TEST_F(CommandLineFixture, testResolveInvalidCommand)
{
   //TS_ASSERT_THROWS(mCmdLine.getArgument(UTEXT("non-existing-argument")), CommandLineException*);
}
