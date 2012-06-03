
#include "testbufferedstream.h"

void TestBufferedStream::tearDown()
{
   mStream.clear();
}

void TestBufferedStream::testReadWriteInt()
{
   mStream.writeInt(5);

   int value = 0;
   mStream.readInt(value);
   TS_ASSERT_EQUALS(value, 5);
}

void TestBufferedStream::testReadWriteFloat()
{
   mStream.writeFloat(5);

   float value = 0.0f;
   mStream.readFloat(value);
   TS_ASSERT_EQUALS(value, 5.0f);
}
