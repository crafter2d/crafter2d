
#include <gtest/gtest.h>

#include "core/math/rect.h"

using namespace c2d;

TEST(Rect, testCenter)
{
   RectF rectf(10, 10, 20, 20);
   auto center = rectf.center();
   EXPECT_EQ(center.x, 20);
   EXPECT_EQ(center.y, 20);
}
