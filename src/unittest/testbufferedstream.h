
#ifndef TEST_BUFFERED_STREAM_H
#define TEST_BUFFERED_STREAM_H

#include <cxxtest/TestSuite.h>

#include "core/streams/bufferedstream.h"

class TestBufferedStream : public CxxTest::TestSuite
{
   BufferedStream mStream;
public:
   void tearDown();

   void testReadWriteInt();
   void testReadWriteFloat();
};

#endif // TEST_BUFFERED_STREAM_H
