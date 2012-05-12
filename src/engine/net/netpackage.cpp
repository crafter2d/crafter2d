/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "netpackage.h"

#include <string.h>

#include "core/system/timer.h"

#include "bitstream.h"

const int NetPackage::MaxPackageSize = sizeof(NetHeader) + sizeof(int) + MaxDataSize;

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
   mData()
{
}

NetPackage::NetPackage(Type type, Reliability reliability, int packagenr, int datasize, const char* pdata):
   mHeader(type, reliability, packagenr),
   mDataSize(0),
   mData()
{
   if ( datasize > 0 )
   {
      setData(datasize, pdata);
   }
}

NetPackage::NetPackage(const NetPackage& that):
   mHeader(that.mHeader),
   mDataSize(0),
   mData()
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

const char* NetPackage::getData() const
{
   return mData;
}

void NetPackage::setData(int datasize, const char* pdata)
{
   ASSERT(datasize > 0);
   ASSERT(datasize < MaxDataSize);

   mDataSize = datasize;
   memmove(mData, pdata, datasize);
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
