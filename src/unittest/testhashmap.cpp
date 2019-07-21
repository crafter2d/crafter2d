
#include <gtest/gtest.h>

#include "core/containers/hashmap.h"

unsigned int hashInt(const int& value)
{
   return value;
}

class Value
{
public:
   Value(int k, int v) : key(k), value(v) {}

   int key;
   int value;
};

TEST(HashMap, testInsertion)
{
   Value test1(1, 100);
   Value test2(2, 200);

   HashMap<int, Value> map;
   map.setHashFunction(hashInt);
   EXPECT_TRUE(map.isEmpty());

   map.insert(test1.key, test1);
   map.insert(test2.key, test2);

   EXPECT_EQ(map.size(), 2);
   EXPECT_FALSE(map.isEmpty());
}
