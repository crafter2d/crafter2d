/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TestHashMap_init = false;
#include "/home/jeroen/Devel/crafter2d/src/unittest/testhashmap.h"

static TestHashMap suite_TestHashMap;

static CxxTest::List Tests_TestHashMap = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestHashMap( "/home/jeroen/Devel/crafter2d/src/unittest/testhashmap.h", 14, "TestHashMap", suite_TestHashMap, Tests_TestHashMap );

static class TestDescription_suite_TestHashMap_testInsertion : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestHashMap_testInsertion() : CxxTest::RealTestDescription( Tests_TestHashMap, suiteDescription_TestHashMap, 26, "testInsertion" ) {}
 void runTest() { suite_TestHashMap.testInsertion(); }
} testDescription_suite_TestHashMap_testInsertion;

#include "/home/jeroen/Devel/crafter2d/src/unittest/testvariant.h"

static TestVariant suite_TestVariant;

static CxxTest::List Tests_TestVariant = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestVariant( "/home/jeroen/Devel/crafter2d/src/unittest/testvariant.h", 9, "TestVariant", suite_TestVariant, Tests_TestVariant );

static class TestDescription_suite_TestVariant_testInt : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestVariant_testInt() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 13, "testInt" ) {}
 void runTest() { suite_TestVariant.testInt(); }
} testDescription_suite_TestVariant_testInt;

static class TestDescription_suite_TestVariant_testReal : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestVariant_testReal() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 23, "testReal" ) {}
 void runTest() { suite_TestVariant.testReal(); }
} testDescription_suite_TestVariant_testReal;

static class TestDescription_suite_TestVariant_testBoolean : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestVariant_testBoolean() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 34, "testBoolean" ) {}
 void runTest() { suite_TestVariant.testBoolean(); }
} testDescription_suite_TestVariant_testBoolean;

static class TestDescription_suite_TestVariant_testString : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestVariant_testString() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 46, "testString" ) {}
 void runTest() { suite_TestVariant.testString(); }
} testDescription_suite_TestVariant_testString;

static class TestDescription_suite_TestVariant_testAssignment : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestVariant_testAssignment() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 55, "testAssignment" ) {}
 void runTest() { suite_TestVariant.testAssignment(); }
} testDescription_suite_TestVariant_testAssignment;

#include "/home/jeroen/Devel/crafter2d/src/unittest/testcommandline.h"

static TestCommandLine suite_TestCommandLine;

static CxxTest::List Tests_TestCommandLine = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestCommandLine( "/home/jeroen/Devel/crafter2d/src/unittest/testcommandline.h", 10, "TestCommandLine", suite_TestCommandLine, Tests_TestCommandLine );

static class TestDescription_suite_TestCommandLine_testSize : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestCommandLine_testSize() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 17, "testSize" ) {}
 void runTest() { suite_TestCommandLine.testSize(); }
} testDescription_suite_TestCommandLine_testSize;

static class TestDescription_suite_TestCommandLine_testSingleCommand : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestCommandLine_testSingleCommand() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 18, "testSingleCommand" ) {}
 void runTest() { suite_TestCommandLine.testSingleCommand(); }
} testDescription_suite_TestCommandLine_testSingleCommand;

static class TestDescription_suite_TestCommandLine_testSpaceSeparatedArgument : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestCommandLine_testSpaceSeparatedArgument() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 19, "testSpaceSeparatedArgument" ) {}
 void runTest() { suite_TestCommandLine.testSpaceSeparatedArgument(); }
} testDescription_suite_TestCommandLine_testSpaceSeparatedArgument;

static class TestDescription_suite_TestCommandLine_testFindValidArgument : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestCommandLine_testFindValidArgument() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 20, "testFindValidArgument" ) {}
 void runTest() { suite_TestCommandLine.testFindValidArgument(); }
} testDescription_suite_TestCommandLine_testFindValidArgument;

static class TestDescription_suite_TestCommandLine_testFindNonExistingArgument : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestCommandLine_testFindNonExistingArgument() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 21, "testFindNonExistingArgument" ) {}
 void runTest() { suite_TestCommandLine.testFindNonExistingArgument(); }
} testDescription_suite_TestCommandLine_testFindNonExistingArgument;

static class TestDescription_suite_TestCommandLine_testResolveCommand : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestCommandLine_testResolveCommand() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 22, "testResolveCommand" ) {}
 void runTest() { suite_TestCommandLine.testResolveCommand(); }
} testDescription_suite_TestCommandLine_testResolveCommand;

static class TestDescription_suite_TestCommandLine_testResolveInvalidCommand : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestCommandLine_testResolveInvalidCommand() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 23, "testResolveInvalidCommand" ) {}
 void runTest() { suite_TestCommandLine.testResolveInvalidCommand(); }
} testDescription_suite_TestCommandLine_testResolveInvalidCommand;

#include "/home/jeroen/Devel/crafter2d/src/unittest/testvirtualmachine.h"

static TestVirtualMachine suite_TestVirtualMachine;

static CxxTest::List Tests_TestVirtualMachine = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestVirtualMachine( "/home/jeroen/Devel/crafter2d/src/unittest/testvirtualmachine.h", 82, "TestVirtualMachine", suite_TestVirtualMachine, Tests_TestVirtualMachine );

static class TestDescription_suite_TestVirtualMachine_testRun : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestVirtualMachine_testRun() : CxxTest::RealTestDescription( Tests_TestVirtualMachine, suiteDescription_TestVirtualMachine, 86, "testRun" ) {}
 void runTest() { suite_TestVirtualMachine.testRun(); }
} testDescription_suite_TestVirtualMachine_testRun;

#include "/home/jeroen/Devel/crafter2d/src/unittest/testbufferedstream.h"

static TestBufferedStream suite_TestBufferedStream;

static CxxTest::List Tests_TestBufferedStream = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestBufferedStream( "/home/jeroen/Devel/crafter2d/src/unittest/testbufferedstream.h", 9, "TestBufferedStream", suite_TestBufferedStream, Tests_TestBufferedStream );

static class TestDescription_suite_TestBufferedStream_testReadWriteInt : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBufferedStream_testReadWriteInt() : CxxTest::RealTestDescription( Tests_TestBufferedStream, suiteDescription_TestBufferedStream, 15, "testReadWriteInt" ) {}
 void runTest() { suite_TestBufferedStream.testReadWriteInt(); }
} testDescription_suite_TestBufferedStream_testReadWriteInt;

static class TestDescription_suite_TestBufferedStream_testReadWriteFloat : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestBufferedStream_testReadWriteFloat() : CxxTest::RealTestDescription( Tests_TestBufferedStream, suiteDescription_TestBufferedStream, 16, "testReadWriteFloat" ) {}
 void runTest() { suite_TestBufferedStream.testReadWriteFloat(); }
} testDescription_suite_TestBufferedStream_testReadWriteFloat;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
