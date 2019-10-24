
#include <gtest/gtest.h>

#include "core/containers/tree.h"
#include "core/string/string.h"

TEST(Tree, testTree)
{
   String test(L"test");
   Tree<String> tree;
   auto it = tree.begin();
   tree.insert_child(it, test);
   EXPECT_TRUE(tree.count() == 2);
}
