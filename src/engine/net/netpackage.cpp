#include "netpackage.h"

#include <string.h>

#include "core/system/timer.h"

#include "bitstream.h"

NetPackage::NetHeader::NetHeader():
   mType(eInvalid),
   mReliability(eUnreliable),
   mNumber(0),
   mTimeStamp(0.0f)
{
}

NetPackage::NetHeader::NetHeader(const NetHeader& that):
   mType(that.mType),
   mReliability(that.mReliability),
   mNumber(that.mNumber),
   mTimeStamp(Timer::getInstance().getTick())
{
}

NetPackage::NetHeader::NetHeader(Type type, Reliability reliability, int packagenr):
   mType(type),
   mReliability(reliability),
   mNumber(packagenr),
   mTimeStamp(Timer::getInstance().getTick())
{
}

NetPackage::NetPackage():
   mHeader(),
   mDataSize(0),
   mpData(NULL)
{
}

NetPackage::NetPackage(Type type, Reliability reliability, int packagenr, int datasize, const char* pdata):
   mHeader(type, reliability, packagenr),
   mDataSize(0),
   mpData(NULL)
{
   if ( datasize > 0 )
   {
      setData(datasize, pdata);
   }
}

NetPackage::NetPackage(const NetPackage& that):
   mHeader(that.mHeader),
   mDataSize(0),
   mpData(NULL)
{
   if ( that.getDataSize() > 0 )
   {
      setData(that.getDataSize(), that.getData());
   }
}

NetPackage::~NetPackage()
{
}

// - Get/set

NetPackage::Type NetPackage::getType() const
{
   return (Type)mHeader.mType;
}

NetPackage::Reliability NetPackage::getReliability() const
{
   return (Reliability)mHeader.mReliability;
}

float NetPackage::getTimeStamp() const
{
   return mHeader.mTimeStamp;
}

void NetPackage::setTimeStamp(float timestamp)
{
   mHeader.mTimeStamp = timestamp;
}

uint NetPackage::getNumber() const
{
   return mHeader.mNumber;
}

int NetPackage::getDataSize() const
{
   return mDataSize;
}

char* NetPackage::getData() const
{
   return mpData;
}

void NetPackage::setData(int datasize, const char* pdata)
{
   ASSERT(mpData == NULL);

   mDataSize = datasize;
   mpData = new char[datasize];
   memmove(mpData, pdata, datasize);
}

// - Query

int NetPackage::getSize() const
{
   return sizeof(NetHeader) + sizeof(int) + (mDataSize * sizeof(char));
}

// - Streaming

/*

NetPackage& NetPackage::operator<<(BitStream& stream)
{
   stream >> mType >> mReliability >> mNumber >> mDataStream;

   return *this;
}

const NetPackage& NetPackage::operator>>(BitStream& stream) const
{
   stream << mType << mReliability << mNumber << &mDataStream;

   return *this;
}
*/