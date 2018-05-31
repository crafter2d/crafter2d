/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#ifndef _NETPACKAGE_H_
#define _NETPACKAGE_H_

#include "core/defines.h"
#include "core/memory/objecthandle.h"

class NetObject;

#define TYPE_MASK 0xFF000000
#define RELI_MASK 0x00FF0000
#define SIZE_MASK 0x0000FFFF

#define TYPE_SHIFT 24
#define RELI_SHIFT 16
#define SIZE_SHIFT 0 

class NetPackage
{
public:
   static const int HeaderSize;
   static const int MaxDataSize = 10000;
   static const int MaxPackageSize;

   enum PacketType {
      eEvent   = 1,
      eAck     = 2,
      eRequest = 3,
      eAlive   = 4,
      eInvalid = -1
   };

   enum Reliability {
      eUnreliable          = 1,
      eReliableSequenced   = 2,
      eReliableOrdered     = 3
   };

   NetPackage();
   NetPackage(const NetPackage& that);
   NetPackage(NetPackage&& that);
   NetPackage(PacketType type, Reliability reliability, int packagenr, int datasize = 0, const char* pdata = nullptr);
   ~NetPackage();

   NetPackage& operator=(NetPackage&& that);

 // get/set
   PacketType getType() const {
      return (PacketType)((mInfo & TYPE_MASK) >> TYPE_SHIFT);
   }

   void setType(PacketType type) {
      mInfo |= (type << TYPE_SHIFT) & TYPE_MASK;
   }

   Reliability getReliability() const {
      return (Reliability)((mInfo & RELI_MASK) >> RELI_SHIFT);
   }
 
   void setReliability(Reliability rel) {
      mInfo |= (rel << RELI_SHIFT) & RELI_MASK;
   }

   uint32_t getNumber() const {
      return mNumber;
   }

   void setNumber(uint32_t number) {
      mNumber = number;
   }

   float getTimeStamp() const {
      return mTimeStamp;
   }

   void setTimeStamp(float timestamp) {
      mTimeStamp = timestamp;
   }

   int getDataSize() const {
      return mInfo & SIZE_MASK;
   }

   const char* getData() const {
      return mData;
   }

   void setData(int datasize, const char* pdata);

 // query
   int         getSize() const;

 // operations
   void        reset();

private:
 // get/set
   void setDataSize(int size) {
      mInfo |= size & SIZE_MASK;
   }


 // data
   float    mTimeStamp;
   uint32_t mNumber;
   uint32_t mInfo;  // packed info: 4 = type, 3 = reliability, 2&1 = datasize
   char     mData[MaxDataSize];
};

typedef ObjectHandle<NetPackage> PackageHandle;

struct lessPackage
{
   bool operator()(const PackageHandle& left, const PackageHandle& right)
   {
      return left->getNumber() < right->getNumber();
   }
};

#endif // _NETPACKAGE_H_
