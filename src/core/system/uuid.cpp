
#include "uuid.h"

#include <string>

#include "core/streams/datastream.h"

namespace c2d
{
   Uuid::Uuid() :
      mData1(0),
      mData2(0),
      mData3(0),
      mData4(0),
      mData5(0)
   {
   }

   Uuid::Uuid(const Uuid& that) :
      mData1(that.mData1),
      mData2(that.mData2),
      mData3(that.mData3),
      mData4(that.mData4),
      mData5(that.mData5)
   {
   }

   Uuid::Uuid(uint32_t data1, uint16_t data2, uint16_t data3, uint16_t data4, uint64_t data5) :
      mData1(data1),
      mData2(data2),
      mData3(data3),
      mData4(data4),
      mData5(data5)
   {
   }

   bool Uuid::operator<(const Uuid& that) const
   {
      return sum() < that.sum();
   }

   bool Uuid::operator==(const Uuid& that) const
   {
      return mData1 == that.mData1
         && mData2 == that.mData2
         && mData3 == that.mData3
         && mData4 == that.mData4
         && mData5 == that.mData5;
   }

   // - Operations

   uint64_t Uuid::sum() const
   {
      return mData1 + mData2 + mData3 + mData4 + mData5;
   }

   // - Streaming

   void Uuid::read(DataStream& stream)
   {
      stream.readUint32(mData1);
      stream.readUint16(mData2);
      stream.readUint16(mData3);
      stream.readUint16(mData4);
      stream.readUint64(mData5);
   }

   void Uuid::write(DataStream& stream) const
   {
      stream.writeUint32(mData1);
      stream.writeUint16(mData2);
      stream.writeUint16(mData3);
      stream.writeUint16(mData4);
      stream.writeUint64(mData5);
   }
}
