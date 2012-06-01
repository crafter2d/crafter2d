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
#ifndef NET_ADDRESS_H
#define NET_ADDRESS_H

#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
#include <vector>

#include "core/memory/objecthandle.h"

#include "netpackage.h"
#include "sortedpackagelist.h"

class NetPackage;
class NetStatistics;

class NetAddress
{
public:
   static const int SOCKADDR_SIZE = sizeof(sockaddr_in);

   typedef std::vector< ObjectHandle<NetPackage> > PackageQueue;

   NetAddress();
   NetAddress(const sockaddr_in& address);

 // operations
   void removeAcknowledged(uint number);

 // members
   int         index;
   sockaddr_in addr;

   uint  packageNumber;
   uint  nextPackageNumber;
   float lastTimeRecv;
   float lastTimeSend;

   int   waitAttempt;
   float waitTimer;

   SortedPackageList orderQueue;
   PackageQueue resendQueue;

   NetStatistics* pstatistics;
};

#endif // NET_ADDRESS_H
