
#ifndef TEST_LIST_H
#define TEST_LIST_H

#include <cxxtest/TestSuite.h>

#include "core/containers/list.h"

class TestList : public CxxTest::TestSuite
{
   class Value
   {
   public:
      Value(int v): value(v) {}

      int value;
   };

   List<Value> list;

public:
   void setUp();
   void tearDown();

   void testSize();
   void testIterator();
   void testClear();
};

#endif // TEST_LIST_H
