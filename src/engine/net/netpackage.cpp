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

const int NetPackage::HeaderSize     = sizeof(float) + sizeof(int) * 2;
const int NetPackage::MaxPackageSize = HeaderSize + MaxDataSize;

#define TYPE_MASK 0xFF000000
#define RELI_MASK 0x00FF0000
#define SIZE_MASK 0x0000FFFF

#define TYPE_SHIFT 24
#define RELI_SHIFT 16
#define SIZE_SHIFT 0 

// - NetPackage implementation

NetPackage::NetPackage():
   mTimeStamp(Timer::getInstance().getTick()),
   mNumber(0),
   mInfo(0),
   mData()
{
}

NetPackage::NetPackage(PacketType type, Reliability reliability, int packagenr, int datasize, const char* pdata):
   mTimeStamp(Timer::getInstance().getTick()),
   mNumber(packagenr),
   mInfo(0),
   mData()
{
   setType(type);
   setReliability(reliability);

   if ( datasize > 0 )
   {
      setData(datasize, pdata);
   }
}

NetPackage::NetPackage(const NetPackage& that):
   mTimeStamp(Timer::getInstance().getTick()),
   mNumber(that.mNumber),
   mInfo(that.mInfo),
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

NetPackage::PacketType NetPackage::getType() const
{
   return (PacketType)((mInfo & TYPE_MASK) >> TYPE_SHIFT);
}

void NetPackage::setType(PacketType type)
{
   mInfo |= (type << TYPE_SHIFT) & TYPE_MASK;
}

NetPackage::Reliability NetPackage::getReliability() const
{
   return (Reliability)((mInfo & RELI_MASK) >> RELI_SHIFT);
}

void NetPackage::setReliability(Reliability rel)
{
   mInfo |= (rel << RELI_SHIFT) & RELI_MASK;
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

void NetPackage::setNumber(uint number)
{
   mNumber = number;
}

int NetPackage::getDataSize() const
{
   return mInfo & SIZE_MASK;
}

void NetPackage::setDataSize(int size)
{
   mInfo |= size & SIZE_MASK;
}

const char* NetPackage::getData() const
{
   return mData;
}

void NetPackage::setData(int datasize, const char* pdata)
{
   ASSERT(datasize > 0);
   ASSERT(datasize < MaxDataSize);

   setDataSize(datasize);
   memmove(mData, pdata, datasize);
}

// - Query

int NetPackage::getSize() const
{
   return HeaderSize + (getDataSize() * sizeof(char));
}

// - Operations

void NetPackage::reset()
{
   mInfo = 0;
}
