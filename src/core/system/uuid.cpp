
#include "uuid.h"

#include <string>

#include "core/streams/datastream.h"

Uuid::Uuid():
   mData1(),
   mData2(),
   mData3(),
   mData4()
{
}

Uuid::Uuid(const Uuid& that):
   mData1(that.mData1),
   mData2(that.mData2),
   mData3(that.mData3),
   mData4()
{
   strncpy((char*)mData4, (const char*)that.mData4, 8);
}

Uuid::Uuid(uint64_t data1, uint16_t data2, uint16_t data3, uint8_t data4[8]) :
   mData1(data1),
   mData2(data2),
   mData3(data3),
   mData4()
{
   strncpy((char*)mData4, (const char*)data4, 8);
}

bool Uuid::operator==(const Uuid& that) const
{
   return mData1 == that.mData1
      && mData2 == that.mData2
      && mData3 == that.mData3
      && strncmp((const char*)mData4, (const char*)that.mData4, 8) == 0;
}

// - Streaming

void Uuid::read(DataStream& stream)
{
   stream.readUint64(mData1);
   stream.readUint16(mData2);
   stream.readUint16(mData3);
   stream.readBlob(mData4, sizeof(uint8_t)* 8);
}

void Uuid::write(DataStream& stream)
{
   stream.writeUint64(mData1);
   stream.writeUint16(mData2);
   stream.writeUint16(mData3);
   stream.writeBlob(mData4, sizeof(uint8_t)* 8);
}
