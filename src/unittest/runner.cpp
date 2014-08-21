/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/StdioPrinter.h>
#include <cxxtest/Win32Gui.h>

int main( int argc, char *argv[] ) {
 return CxxTest::GuiTuiRunner<CxxTest::Win32Gui, CxxTest::StdioPrinter>( argc, argv ).run();
}
#include "F:/Devel/crafter2d/src/unittest/testbufferedstream.h"

static TestBufferedStream suite_TestBufferedStream;

static CxxTest::List Tests_TestBufferedStream = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestBufferedStream( "F:/Devel/crafter2d/src/unittest/testbufferedstream.h", 9, "TestBufferedStream", suite_TestBufferedStream, Tests_TestBufferedStream );

static class TestDescription_TestBufferedStream_testReadWriteInt : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBufferedStream_testReadWriteInt() : CxxTest::RealTestDescription( Tests_TestBufferedStream, suiteDescription_TestBufferedStream, 15, "testReadWriteInt" ) {}
 void runTest() { suite_TestBufferedStream.testReadWriteInt(); }
} testDescription_TestBufferedStream_testReadWriteInt;

static class TestDescription_TestBufferedStream_testReadWriteFloat : public CxxTest::RealTestDescription {
public:
 TestDescription_TestBufferedStream_testReadWriteFloat() : CxxTest::RealTestDescription( Tests_TestBufferedStream, suiteDescription_TestBufferedStream, 16, "testReadWriteFloat" ) {}
 void runTest() { suite_TestBufferedStream.testReadWriteFloat(); }
} testDescription_TestBufferedStream_testReadWriteFloat;

#include "F:/Devel/crafter2d/src/unittest/testcommandline.h"

static TestCommandLine suite_TestCommandLine;

static CxxTest::List Tests_TestCommandLine = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestCommandLine( "F:/Devel/crafter2d/src/unittest/testcommandline.h", 10, "TestCommandLine", suite_TestCommandLine, Tests_TestCommandLine );

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

#include "F:/Devel/crafter2d/src/unittest/testhashmap.h"

static TestHashMap suite_TestHashMap;

static CxxTest::List Tests_TestHashMap = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestHashMap( "F:/Devel/crafter2d/src/unittest/testhashmap.h", 14, "TestHashMap", suite_TestHashMap, Tests_TestHashMap );

static class TestDescription_TestHashMap_testInsertion : public CxxTest::RealTestDescription {
public:
 TestDescription_TestHashMap_testInsertion() : CxxTest::RealTestDescription( Tests_TestHashMap, suiteDescription_TestHashMap, 26, "testInsertion" ) {}
 void runTest() { suite_TestHashMap.testInsertion(); }
} testDescription_TestHashMap_testInsertion;

#include "F:/Devel/crafter2d/src/unittest/testlist.h"

static TestList suite_TestList;

static CxxTest::List Tests_TestList = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestList( "F:/Devel/crafter2d/src/unittest/testlist.h", 9, "TestList", suite_TestList, Tests_TestList );

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

#include "F:/Devel/crafter2d/src/unittest/testvariant.h"

static TestVariant suite_TestVariant;

static CxxTest::List Tests_TestVariant = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestVariant( "F:/Devel/crafter2d/src/unittest/testvariant.h", 9, "TestVariant", suite_TestVariant, Tests_TestVariant );

static class TestDescription_TestVariant_testInt : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVariant_testInt() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 13, "testInt" ) {}
 void runTest() { suite_TestVariant.testInt(); }
} testDescription_TestVariant_testInt;

static class TestDescription_TestVariant_testReal : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVariant_testReal() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 23, "testReal" ) {}
 void runTest() { suite_TestVariant.testReal(); }
} testDescription_TestVariant_testReal;

static class TestDescription_TestVariant_testBoolean : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVariant_testBoolean() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 34, "testBoolean" ) {}
 void runTest() { suite_TestVariant.testBoolean(); }
} testDescription_TestVariant_testBoolean;

static class TestDescription_TestVariant_testString : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVariant_testString() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 46, "testString" ) {}
 void runTest() { suite_TestVariant.testString(); }
} testDescription_TestVariant_testString;

static class TestDescription_TestVariant_testAssignment : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVariant_testAssignment() : CxxTest::RealTestDescription( Tests_TestVariant, suiteDescription_TestVariant, 55, "testAssignment" ) {}
 void runTest() { suite_TestVariant.testAssignment(); }
} testDescription_TestVariant_testAssignment;

#include "F:/Devel/crafter2d/src/unittest/testvirtualmachine.h"

static TestVirtualMachine suite_TestVirtualMachine;

static CxxTest::List Tests_TestVirtualMachine = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestVirtualMachine( "F:/Devel/crafter2d/src/unittest/testvirtualmachine.h", 82, "TestVirtualMachine", suite_TestVirtualMachine, Tests_TestVirtualMachine );

static class TestDescription_TestVirtualMachine_testRun : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVirtualMachine_testRun() : CxxTest::RealTestDescription( Tests_TestVirtualMachine, suiteDescription_TestVirtualMachine, 86, "testRun" ) {}
 void runTest() { suite_TestVirtualMachine.testRun(); }
} testDescription_TestVirtualMachine_testRun;

#include <cxxtest/Root.cpp>
