
#ifndef TEST_VARIANT_H
#define TEST_VARIANT_H

#include <unittest++/UnitTest++.h>

#include "core/system/variant.h"

std::ostream& operator<<(std::ostream& stream, const String& str)
{
   return stream << str.toUtf8();
}

SUITE(TestVariant)
{
   TEST(testInt)
   {
      Variant v(100);

      CHECK(v.isInt());
      CHECK(v.asInt() == 100);
      CHECK_EQUAL(100.0f, v.toReal());
      CHECK_EQUAL(UTEXT("100"), v.toString());
   }

   TEST(testReal)
   {
      float i = 100.0;
      Variant v(i);

      CHECK(v.isReal());
      CHECK_EQUAL(100.0f, v.asReal());
      CHECK_EQUAL(100, v.toInt());
      CHECK_EQUAL(UTEXT("100"), v.toString());
   }

   TEST(testBoolean)
   {
      Variant vtrue(true);

      CHECK(vtrue.isBool());
      CHECK(vtrue.asBool());
      CHECK_EQUAL(vtrue.toString(), UTEXT("true"));

      Variant vfalse(false);
      CHECK_EQUAL(vfalse.toString(), UTEXT("false"));
   }

   TEST(testString)
   {
      String value(UTEXT("100"));
      Variant v(value);
      CHECK(v.isString());
      CHECK(v.toInt() == 100);
   }

   TEST(testAssignment)
   {
      Variant a(100);
      Variant b;
      b = a;

      CHECK(a.isInt());
      CHECK(b.isInt());

      CHECK_EQUAL(a.asInt(), b.asInt());
   }
};

#endif // TEST_VARIANT_H
