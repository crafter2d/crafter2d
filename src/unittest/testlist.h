
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

   List<Value> mList;

public:
   void setUp();
   void tearDown();
   
   void testAddFront();
   void testAddTail();
   void testInsert();
   void testGetFront();
   void testGetTail();
   void testRandomAccess();
   void testSize();
   void testIteratorIncrement();
   void testIteratorRemove();
   void testIteratorInvalidRemove();
   void testClear();
};

#endif // TEST_LIST_H
