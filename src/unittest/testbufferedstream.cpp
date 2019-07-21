
#include <gtest/gtest.h>

#include "core/streams/bufferedstream.h"

TEST(BufferedStream, testReadWriteInt)
{
   BufferedStream stream;

   stream.writeInt(5);

   int value = 0;
   stream.readInt(value);
   EXPECT_EQ(value, 5);
}

TEST(BufferedStream, testReadWriteFloat)
{
   BufferedStream stream;
   stream.writeFloat(5);

   float value = 0.0f;
   stream.readFloat(value);
   EXPECT_FLOAT_EQ(value, 5.0f);
}
