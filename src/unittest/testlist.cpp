
#include "testlist.h"

#include "core/containers/listiterator.h"
#include "core/containers/listalgorithms.h"

// - Fixtures

void TestList::setUp() 
{
   Value* pvalue = new Value(3);
   mList.addTail(pvalue);
   pvalue = new Value(5);
   mList.addTail(pvalue);
}

void TestList::tearDown()
{
   ListAlgorithms::flush<Value*>(mList);
}

// - Tests

void TestList::testAddFront()
{
   Value* pvalue = new Value(1);
   mList.addFront(pvalue);

   TS_ASSERT(mList.size() == 3);
   TS_ASSERT(mList.getFront().item()->value == 1);
}

void TestList::testAddTail()
{
   Value* pvalue = new Value(7);
   mList.addFront(pvalue);

   TS_ASSERT(mList.size() == 3);
   TS_ASSERT(mList.getTail().item()->value == 1);
}

void TestList::testSize()
{
   TS_ASSERT_EQUALS(mList.size(), 2);
}

void TestList::testIteratorIncrement()
{
   ListIterator<Value*> it = mList.getFront();

   TS_ASSERT_EQUALS(it.item()->value, 3);
   TS_ASSERT_EQUALS((*it)->value, 3);

   ++it;

   TS_ASSERT(it.isValid());
   TS_ASSERT_EQUALS((*it)->value, 5);

   ++it;
   TS_ASSERT(!it.isValid());

   // test what happens when the iterator is invalid already
   TS_ASSERT_THROWS_NOTHING(++it);
}

void TestList::testIteratorRemove()
{
   ListIterator<Value*> it = mList.getFront();
   ++it;

   TS_ASSERT(it.isValid() && it.item()->value == 5);

   it.remove();

   TS_ASSERT(!it.isValid());
   TS_ASSERT(mList.size() == 1);
}

void TestList::testClear()
{
   TS_ASSERT_EQUALS(mList.isEmpty(), false);

   ListAlgorithms::flush<Value*>(mList);

   TS_ASSERT(mList.isEmpty());
   TS_ASSERT_EQUALS(mList.size(), 0);
}