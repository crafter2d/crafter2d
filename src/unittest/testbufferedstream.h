
#ifndef TEST_BUFFERED_STREAM_H
#define TEST_BUFFERED_STREAM_H

#include <unittest++/UnitTest++.h>

#include "core/streams/bufferedstream.h"

SUITE(TestBufferedStream)
{
   class Fixture
   {
   public:
      BufferedStream mStream;
   };

   TEST_FIXTURE(Fixture, testReadWriteInt)
   {
      mStream.writeInt(5);

      int value = 0;
      mStream.readInt(value);
      CHECK_EQUAL(value, 5);
   }

   TEST_FIXTURE(Fixture, testReadWriteFloat)
   {
      mStream.writeFloat(5);

      float value = 0.0f;
      mStream.readFloat(value);
      CHECK_EQUAL(value, 5.0f);
   }
};

#endif // TEST_BUFFERED_STREAM_H
