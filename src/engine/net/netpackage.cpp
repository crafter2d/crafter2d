#include "netpackage.h"

#include <string.h>

#include "core/system/timer.h"

#include "bitstream.h"
#include "netevent.h"

NetPackage::NetPackage():
   mType(eInvalid),
   mReliability(eUnreliable),
   mNumber(0),
   mTimeStamp(0.0f),
   mDataStream()
{
}

NetPackage::NetPackage(Type type, Reliability reliability, int packagenr, int datasize, const char* pdata):
   mType(type),
   mReliability(reliability),
   mNumber(packagenr),
   mTimeStamp(Timer::getInstance().getTick()),
   mDataStream()
{
   if ( datasize > 0 )
   {
      mDataStream.setBuffer(pdata, datasize);
   }
}

NetPackage::NetPackage(const NetPackage& that):
   mType(that.mType),
   mReliability(that.mReliability),
   mNumber(that.mNumber),
   mTimeStamp(that.mTimeStamp),
   mDataStream(that.mDataStream)
{
}

NetPackage::~NetPackage()
{
}

NetPackage::Type NetPackage::getType() const
{
   return (Type)mType;
}

NetPackage::Reliability NetPackage::getReliability() const
{
   return (Reliability)mReliability;
}

float NetPackage::getTimeStamp() const
{
   return mTimeStamp;
}

void NetPackage::setTimeStamp(float timestamp)
{
   mTimeStamp = timestamp;
}

uint NetPackage::getNumber() const
{
   return mNumber;
}

NetObject* NetPackage::getObject()
{
   NetObject* pobject = NULL;

   mDataStream >> &pobject;

   return pobject;
}

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
