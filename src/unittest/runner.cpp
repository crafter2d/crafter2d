/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/StdioPrinter.h>
#include <cxxtest/Win32Gui.h>

int main( int argc, char *argv[] ) {
 return CxxTest::GuiTuiRunner<CxxTest::Win32Gui, CxxTest::StdioPrinter>( argc, argv ).run();
}
#include "D:/Devel/jenginesse/trunk/src/unittest/testcommandline.h"

static TestCommandLine suite_TestCommandLine;

static CxxTest::List Tests_TestCommandLine = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestCommandLine( "D:/Devel/jenginesse/trunk/src/unittest/testcommandline.h", 10, "TestCommandLine", suite_TestCommandLine, Tests_TestCommandLine );

static class TestDescription_TestCommandLine_testSize : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCommandLine_testSize() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 17, "testSize" ) {}
 void runTest() { suite_TestCommandLine.testSize(); }
} testDescription_TestCommandLine_testSize;

static class TestDescription_TestCommandLine_testSingleCommand : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCommandLine_testSingleCommand() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 18, "testSingleCommand" ) {}
 void runTest() { suite_TestCommandLine.testSingleCommand(); }
} testDescription_TestCommandLine_testSingleCommand;

static class TestDescription_TestCommandLine_testSpaceSeparatedArgument : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCommandLine_testSpaceSeparatedArgument() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 19, "testSpaceSeparatedArgument" ) {}
 void runTest() { suite_TestCommandLine.testSpaceSeparatedArgument(); }
} testDescription_TestCommandLine_testSpaceSeparatedArgument;

static class TestDescription_TestCommandLine_testFindValidArgument : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCommandLine_testFindValidArgument() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 20, "testFindValidArgument" ) {}
 void runTest() { suite_TestCommandLine.testFindValidArgument(); }
} testDescription_TestCommandLine_testFindValidArgument;

static class TestDescription_TestCommandLine_testFindNonExistingArgument : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCommandLine_testFindNonExistingArgument() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 21, "testFindNonExistingArgument" ) {}
 void runTest() { suite_TestCommandLine.testFindNonExistingArgument(); }
} testDescription_TestCommandLine_testFindNonExistingArgument;

static class TestDescription_TestCommandLine_testResolveCommand : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCommandLine_testResolveCommand() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 22, "testResolveCommand" ) {}
 void runTest() { suite_TestCommandLine.testResolveCommand(); }
} testDescription_TestCommandLine_testResolveCommand;

static class TestDescription_TestCommandLine_testResolveInvalidCommand : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCommandLine_testResolveInvalidCommand() : CxxTest::RealTestDescription( Tests_TestCommandLine, suiteDescription_TestCommandLine, 23, "testResolveInvalidCommand" ) {}
 void runTest() { suite_TestCommandLine.testResolveInvalidCommand(); }
} testDescription_TestCommandLine_testResolveInvalidCommand;

#include "D:/Devel/jenginesse/trunk/src/unittest/testcompiler.h"

static TestCompiler suite_TestCompiler;

static CxxTest::List Tests_TestCompiler = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestCompiler( "D:/Devel/jenginesse/trunk/src/unittest/testcompiler.h", 12, "TestCompiler", suite_TestCompiler, Tests_TestCompiler );

static class TestDescription_TestCompiler_testCompilation : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCompiler_testCompilation() : CxxTest::RealTestDescription( Tests_TestCompiler, suiteDescription_TestCompiler, 30, "testCompilation" ) {}
 void runTest() { suite_TestCompiler.testCompilation(); }
} testDescription_TestCompiler_testCompilation;

static class TestDescription_TestCompiler_testCallback : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCompiler_testCallback() : CxxTest::RealTestDescription( Tests_TestCompiler, suiteDescription_TestCompiler, 49, "testCallback" ) {}
 void runTest() { suite_TestCompiler.testCallback(); }
} testDescription_TestCompiler_testCallback;

#include "D:/Devel/jenginesse/trunk/src/unittest/testlist.h"

static TestList suite_TestList;

static CxxTest::List Tests_TestList = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestList( "D:/Devel/jenginesse/trunk/src/unittest/testlist.h", 9, "TestList", suite_TestList, Tests_TestList );

static class TestDescription_TestList_testAddFront : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testAddFront() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 25, "testAddFront" ) {}
 void runTest() { suite_TestList.testAddFront(); }
} testDescription_TestList_testAddFront;

static class TestDescription_TestList_testAddTail : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testAddTail() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 26, "testAddTail" ) {}
 void runTest() { suite_TestList.testAddTail(); }
} testDescription_TestList_testAddTail;

static class TestDescription_TestList_testInsert : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testInsert() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 27, "testInsert" ) {}
 void runTest() { suite_TestList.testInsert(); }
} testDescription_TestList_testInsert;

static class TestDescription_TestList_testGetFront : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testGetFront() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 28, "testGetFront" ) {}
 void runTest() { suite_TestList.testGetFront(); }
} testDescription_TestList_testGetFront;

static class TestDescription_TestList_testGetTail : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testGetTail() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 29, "testGetTail" ) {}
 void runTest() { suite_TestList.testGetTail(); }
} testDescription_TestList_testGetTail;

static class TestDescription_TestList_testRandomAccess : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testRandomAccess() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 30, "testRandomAccess" ) {}
 void runTest() { suite_TestList.testRandomAccess(); }
} testDescription_TestList_testRandomAccess;

static class TestDescription_TestList_testSize : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testSize() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 31, "testSize" ) {}
 void runTest() { suite_TestList.testSize(); }
} testDescription_TestList_testSize;

static class TestDescription_TestList_testIteratorIncrement : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testIteratorIncrement() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 32, "testIteratorIncrement" ) {}
 void runTest() { suite_TestList.testIteratorIncrement(); }
} testDescription_TestList_testIteratorIncrement;

static class TestDescription_TestList_testIteratorRemove : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testIteratorRemove() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 33, "testIteratorRemove" ) {}
 void runTest() { suite_TestList.testIteratorRemove(); }
} testDescription_TestList_testIteratorRemove;

static class TestDescription_TestList_testIteratorInvalidRemove : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testIteratorInvalidRemove() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 34, "testIteratorInvalidRemove" ) {}
 void runTest() { suite_TestList.testIteratorInvalidRemove(); }
} testDescription_TestList_testIteratorInvalidRemove;

static class TestDescription_TestList_testClear : public CxxTest::RealTestDescription {
public:
 TestDescription_TestList_testClear() : CxxTest::RealTestDescription( Tests_TestList, suiteDescription_TestList, 35, "testClear" ) {}
 void runTest() { suite_TestList.testClear(); }
} testDescription_TestList_testClear;

#include "D:/Devel/jenginesse/trunk/src/unittest/testvariant.h"

static TestVariant suite_TestVariant;

static CxxTest::List Tests_TestVariant = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestVariant( "D:/Devel/jenginesse/trunk/src/unittest/testvariant.h", 9, "TestVariant", suite_TestVariant, Tests_TestVariant );

static class TestDescription_TestVariant_testInt : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVariant_testInt() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 13, "testInt" ) {}
 void runTest() { suite_TestVariant.testInt(); }
} testDescription_TestVariant_testInt;

static class TestDescription_TestVariant_testString : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVariant_testString() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 27, "testString" ) {}
 void runTest() { suite_TestVariant.testString(); }
} testDescription_TestVariant_testString;

static class TestDescription_TestVariant_testAssignment : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVariant_testAssignment() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 35, "testAssignment" ) {}
 void runTest() { suite_TestVariant.testAssignment(); }
} testDescription_TestVariant_testAssignment;

#include "D:/Devel/jenginesse/trunk/src/unittest/testvirtualmachine.h"

static TestVirtualMachine suite_TestVirtualMachine;

static CxxTest::List Tests_TestVirtualMachine = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestVirtualMachine( "D:/Devel/jenginesse/trunk/src/unittest/testvirtualmachine.h", 11, "TestVirtualMachine", suite_TestVirtualMachine, Tests_TestVirtualMachine );

static class TestDescription_TestVirtualMachine_testRun : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVirtualMachine_testRun() : CxxTest::RealTestDescription( Tests_TestVirtualMachine, suiteDescription_TestVirtualMachine, 15, "testRun" ) {}
 void runTest() { suite_TestVirtualMachine.testRun(); }
} testDescription_TestVirtualMachine_testRun;

#include <cxxtest/Root.cpp>
