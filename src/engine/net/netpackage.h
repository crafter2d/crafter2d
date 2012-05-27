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

class NetPackage
{
public:
   static const int HeaderSize;
   static const int MaxDataSize = 1500;
   static const int MaxPackageSize;

   enum PacketType {
      eEvent   = 1,
      eAck     = 2,
      eAlive   = 3,
      eInvalid = -1
   };

   enum Reliability {
      eUnreliable          = 1,
      eReliableSequenced   = 2,
      eReliableOrdered     = 3
   };

   NetPackage();
   NetPackage(const NetPackage& that);
   NetPackage(PacketType type, Reliability reliability, int packagenr, int datasize = 0, const char* pdata = NULL);
   ~NetPackage();

 // get/set
   PacketType  getType() const;
   void        setType(PacketType type);

   Reliability getReliability() const;
   void        setReliability(Reliability rel);

   uint        getNumber() const;
   void        setNumber(uint number);

   float       getTimeStamp() const;
   void        setTimeStamp(float timestamp);

   int         getDataSize() const;
   const char* getData() const;
   void        setData(int datasize, const char* pdata);

 // query
   int         getSize() const;

 // operations
   void        reset();

private:
 // get/set
   void        setDataSize(int size);

 // data
   float mTimeStamp;
   uint  mNumber;
   uint  mInfo;  // packed info: 4 = type, 3 = reliability, 2&1 = datasize
   char  mData[MaxDataSize];
};

typedef ObjectHandle<NetPackage> PackageHandle;

#endif // _NETPACKAGE_H_
