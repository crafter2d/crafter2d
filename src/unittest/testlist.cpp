
#include "testlist.h"

#include "core/containers/listiterator.h"
#include "core/containers/listalgorithms.h"

// - Fixtures

void TestList::setUp() 
{
   list.addTail(new Value(3));
   list.addTail(new Value(5));
}

void TestList::tearDown()
{
   ListAlgorithms::Flush<Value>(list);
}

// - Tests

void TestList::testSize()
{
   TS_ASSERT_EQUALS(list.size(), 2);
}

void TestList::testIterator()
{
   ListIterator<Value> it(list);

   TS_ASSERT_EQUALS(it.item().value, 3);
   TS_ASSERT_EQUALS((*it).value, 3);

   ++it;

   TS_ASSERT(it.isValid());
   TS_ASSERT_EQUALS((*it).value, 5);

   ++it;
   TS_ASSERT(!it.isValid());

   // test what happens when the iterator is invalid already
   TS_ASSERT_THROWS_NOTHING(++it);
}

void TestList::testClear()
{
   TS_ASSERT_EQUALS(list.isEmpty(), false);

   ListAlgorithms::Flush<Value>(list);

   TS_ASSERT(list.isEmpty());
   TS_ASSERT_EQUALS(list.size(), 0);
}