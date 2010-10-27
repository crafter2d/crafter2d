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

#include "bitstream.h"

class BitStream;
class NetObject;

class NetPackage
{
public:
   enum Type {
      eEvent,
      eAck,
      eAlive,
      eInvalid
   };

   enum Reliability {
      eUnreliable = 0,
      eUnreliableSequenced = 1,
      eReliableSequenced = 2,
      eReliableOrdered = 3
   };

   NetPackage();
   NetPackage(const NetPackage& that);
   NetPackage(Type type, Reliability reliability, int packagenr, int datasize = 0, const char* pdata = NULL);
   ~NetPackage();

   Type        getType() const;
   Reliability getReliability() const;
   uint        getNumber() const;
   float       getTimeStamp() const;
   void        setTimeStamp(float timestamp);

   NetObject*  getObject();

         NetPackage& operator<<(BitStream& stream);
   const NetPackage& operator>>(BitStream& stream) const;

private:

   char        mType;
   char        mReliability;
   uint        mNumber;
   float       mTimeStamp;
   BitStream   mDataStream;
};

#endif
