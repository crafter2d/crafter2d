
#ifndef TEST_VARIANT_H
#define TEST_VARIANT_H

#include <cxxtest/TestSuite.h>

#include "script/common/variant.h"

class TestVariant : public CxxTest::TestSuite
{
public:

   void testInt()
   {
      Variant v(100);
      TS_ASSERT(v.isInt());
      TS_ASSERT(v.asInt() == 100);

      v = Variant(200);
      TS_ASSERT(v.asInt() == 200);

      v.int2string();
      TS_ASSERT(v.isString());
      TS_ASSERT(v.asString() == "200");
   }

   void testString()
   {
      Variant v(std::string("100"));
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
