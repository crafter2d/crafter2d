
#include "testlist.h"

#include "core/containers/listiterator.h"
#include "core/containers/listalgorithms.h"

// - Fixtures

void TestList::setUp() 
{
   mList.addTail(Value(3));
   mList.addTail(Value(5));
}

void TestList::tearDown()
{
   mList.clear();
}

// - Tests

void TestList::testAddFront()
{
   mList.addFront(Value(1));

   TS_ASSERT(mList.size() == 3);
   TS_ASSERT(mList.getFront().item().value == 1);
}

void TestList::testAddTail()
{
   mList.addTail(Value(7));

   TS_ASSERT(mList.size() == 3);
   TS_ASSERT(mList.getTail().item().value == 7);
}

void TestList::testInsert()
{
   ListIterator<Value> it = mList.getFront();
   mList.insert(it, Value(2));
   TS_ASSERT(mList.getFront().item().value == 2);

   ++it;
   mList.insert(it, Value(4));
   TS_ASSERT(mList.get(2).item().value == 4);

   ++it;
   TS_ASSERT(!it.isValid());
   mList.insert(it, Value(6));
}

void TestList::testGetFront()
{
   ListIterator<Value> it = mList.getFront();
   TS_ASSERT(it.isValid());
   TS_ASSERT(it.item().value == 3);
}

void TestList::testGetTail()
{
   ListIterator<Value> it = mList.getTail();
   TS_ASSERT(it.isValid());
   TS_ASSERT(it.item().value == 5);
}

void TestList::testRandomAccess()
{
   ListIterator<Value> it = mList.get(1);
   TS_ASSERT(it.isValid());
   TS_ASSERT(it.item().value == 5);
}

void TestList::testSize()
{
   TS_ASSERT_EQUALS(mList.size(), 2);
}

void TestList::testIteratorIncrement()
{
   ListIterator<Value> it = mList.getFront();

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

void TestList::testIteratorRemove()
{
   ListIterator<Value> it = mList.getFront();
   ++it;

   TS_ASSERT(it.isValid() && it.item().value == 5);

   it.remove();

   TS_ASSERT(!it.isValid());
   TS_ASSERT(mList.size() == 1);
}

void TestList::testIteratorInvalidRemove()
{
   ListIterator<Value> it = mList.getTail();
   ++it;

   TS_ASSERT(!it.isValid());

   it.remove();

   TS_ASSERT(!it.isValid());
}

void TestList::testClear()
{
   List<Value*> list;
   Value* pvalue = new Value(1);
   list.addTail(pvalue);
   pvalue = new Value(2);
   list.addTail(pvalue);

   TS_ASSERT(!list.isEmpty());

   ListAlgorithms::flush<Value*>(list);

   TS_ASSERT(list.isEmpty());
   TS_ASSERT_EQUALS(list.size(), 0);
}
