
#include <gtest/gtest.h>

#include "core/containers/tree.h"
#include "core/string/string.h"

TEST(Tree, testTree)
{
   Tree<String> tree;
   auto it = tree.begin();
   tree.insert_child(it, UTEXT("test1"));
   tree.insert_child(it, UTEXT("test2"));
   EXPECT_TRUE(tree.count() == 3);
}

TEST(Tree, testTreeIterator)
{
   static String test1(L"test1");
   static String test2(L"test2");

   Tree<String> tree;
   auto it = tree.begin();
   tree.insert_child(it, test1);
   ++it;

   tree.insert_child(it, test2);

   EXPECT_TRUE(*it == test1);

   ++it;
   EXPECT_TRUE(*it == test2);

   ++it;
   EXPECT_TRUE(it == tree.end());
}
