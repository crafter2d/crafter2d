
#ifndef TEST_VARIANT_H
#define TEST_VARIANT_H

#include <cxxtest/TestSuite.h>

#include "core/system/variant.h"

class TestVariant : public CxxTest::TestSuite
{
public:

   void testInt()
   {
      Variant v(100);

      TS_ASSERT(v.isInt());
      TS_ASSERT(v.asInt() == 100);
      TS_ASSERT_EQUALS(v.toReal(), 100.0);
      TS_ASSERT_EQUALS(v.toString(), UTEXT("100"));
   }

   void testReal()
   {
      double i = 100.0;
      Variant v(i);

      TS_ASSERT(v.isReal());
      TS_ASSERT_EQUALS(v.asReal(), 100.0);
      TS_ASSERT_EQUALS(v.toInt(), 100);
      TS_ASSERT_EQUALS(v.toString(), UTEXT("100"));
   }

   void testBoolean()
   {
      Variant vtrue(true);

      TS_ASSERT(vtrue.isBool());
      TS_ASSERT(vtrue.asBool());
      TS_ASSERT_EQUALS(vtrue.toString(), UTEXT("true"));

      Variant vfalse(false);
      TS_ASSERT_EQUALS(vfalse.toString(), UTEXT("false"));
   }

   void testString()
   {
      String value(UTEXT("100"));
      Variant v(value);
      TS_ASSERT(v.isString());
      
      TS_ASSERT(v.toInt() == 100);
   }

   void testAssignment()
   {
      Variant a(100);
      Variant b;
      b = a;

      TS_ASSERT(a.isInt());
      TS_ASSERT(b.isInt());

      TS_ASSERT(a.asInt() == b.asInt());
   }
};

#endif // TEST_VARIANT_H
