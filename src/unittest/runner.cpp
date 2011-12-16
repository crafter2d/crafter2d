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
#include "E:/Devel/jengine_sse/trunk/src/unittest/testcompiler.h"

static TestCompiler suite_TestCompiler;

static CxxTest::List Tests_TestCompiler = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestCompiler( "E:/Devel/jengine_sse/trunk/src/unittest/testcompiler.h", 11, "TestCompiler", suite_TestCompiler, Tests_TestCompiler );

static class TestDescription_TestCompiler_testCompilation : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCompiler_testCompilation() : CxxTest::RealTestDescription( Tests_TestCompiler, suiteDescription_TestCompiler, 15, "testCompilation" ) {}
 void runTest() { suite_TestCompiler.testCompilation(); }
} testDescription_TestCompiler_testCompilation;

#include "E:/Devel/jengine_sse/trunk/src/unittest/testvariant.h"

static TestVariant suite_TestVariant;

static CxxTest::List Tests_TestVariant = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestVariant( "E:/Devel/jengine_sse/trunk/src/unittest/testvariant.h", 9, "TestVariant", suite_TestVariant, Tests_TestVariant );

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

#include "E:/Devel/jengine_sse/trunk/src/unittest/testvirtualmachine.h"

static TestVirtualMachine suite_TestVirtualMachine;

static CxxTest::List Tests_TestVirtualMachine = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestVirtualMachine( "E:/Devel/jengine_sse/trunk/src/unittest/testvirtualmachine.h", 11, "TestVirtualMachine", suite_TestVirtualMachine, Tests_TestVirtualMachine );

static class TestDescription_TestVirtualMachine_testRun : public CxxTest::RealTestDescription {
public:
 TestDescription_TestVirtualMachine_testRun() : CxxTest::RealTestDescription( Tests_TestVirtualMachine, suiteDescription_TestVirtualMachine, 15, "testRun" ) {}
 void runTest() { suite_TestVirtualMachine.testRun(); }
} testDescription_TestVirtualMachine_testRun;

#include <cxxtest/Root.cpp>
