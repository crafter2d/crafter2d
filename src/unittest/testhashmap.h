
#ifndef TEST_HASHMAP_H
#define TEST_HASHMAP_H

#include <cxxtest/TestSuite.h>

#include "core/containers/hashmap.h"

unsigned int hashInt(const int& value)
{
   return value;
}

class TestHashMap : public CxxTest::TestSuite
{
   class Test
   {
   public:
      Test(int k, int v): key(k), value(v) {}

      int key;
      int value;
   };

public:
   void testInsertion()
   {
      Test test1(1, 100);
      Test test2(2, 200);

      HashMap<int,Test> map;
      map.setHashFunction(hashInt);
      TS_ASSERT(map.isEmpty());

      map.insert(test1.key, test1);
      map.insert(test2.key, test2);

      TS_ASSERT_EQUALS(map.size(), 2);
      TS_ASSERT(!map.isEmpty());
   }
};

#endif // TEST_HASHMAP_H
