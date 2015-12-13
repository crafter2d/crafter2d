
#ifndef TEST_HASHMAP_H
#define TEST_HASHMAP_H

#include <unittest++/UnitTest++.h>

#include "core/containers/hashmap.h"

unsigned int hashInt(const int& value)
{
   return value;
}

SUITE(TestHashMap)
{
   class Value
   {
   public:
      Value(int k, int v): key(k), value(v) {}

      int key;
      int value;
   };

   TEST(testInsertion)
   {
      Value test1(1, 100);
      Value test2(2, 200);

      HashMap<int, Value> map;
      map.setHashFunction(hashInt);
      CHECK(map.isEmpty());

      map.insert(test1.key, test1);
      map.insert(test2.key, test2);

      CHECK_EQUAL(map.size(), 2);
      CHECK(!map.isEmpty());
   }
};

#endif // TEST_HASHMAP_H
