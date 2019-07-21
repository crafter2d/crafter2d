
#include <gtest/gtest.h>

#include "core/system/variant.h"

std::ostream& operator<<(std::ostream& stream, const String& str)
{
   return stream << str.toUtf8();
}

TEST(Variant, testInt)
{
   Variant v(100);

   EXPECT_TRUE(v.isInt());
   EXPECT_EQ(v.asInt(), 100);
   EXPECT_EQ(v.toReal(), 100.0f);
   EXPECT_EQ(v.toString(), UTEXT("100"));
}

TEST(Variant, testReal)
{
   float i = 100.0;
   Variant v(i);

   EXPECT_TRUE(v.isReal());
   EXPECT_EQ(v.asReal(), 100.0f);
   EXPECT_EQ(v.toInt(), 100);
   EXPECT_EQ(UTEXT("100"), v.toString());
}

TEST(Variant, testBoolean)
{
   Variant vtrue(true);

   EXPECT_TRUE(vtrue.isBool());
   EXPECT_TRUE(vtrue.asBool());
   EXPECT_EQ(vtrue.toString(), UTEXT("true"));

   Variant vfalse(false);
   EXPECT_EQ(vfalse.toString(), UTEXT("false"));
}

TEST(Variant, testString)
{
   String value(UTEXT("100"));
   Variant v(value);
   EXPECT_TRUE(v.isString());
   EXPECT_EQ(v.toInt(), 100);
}

TEST(Variant, testAssignment)
{
   Variant a(100);
   Variant b;
   b = a;

   EXPECT_TRUE(a.isInt());
   EXPECT_TRUE(b.isInt());

   EXPECT_EQ(a.asInt(), b.asInt());
}