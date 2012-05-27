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
#include "netaddress.h"

INLINE
NetAddress::NetAddress():
   packageNumber(1),
   nextPackageNumber(1),
   lastTimeRecv(0),
   lastTimeSend(0),
   waitAttempt(0),
   waitTimer(0),
   orderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memset(&addr, 0, SOCKADDR_SIZE);
   addr.sin_family = AF_INET;
}

INLINE
NetAddress::NetAddress(const sockaddr_in& address):
   packageNumber(1),
   nextPackageNumber(1),
   lastTimeRecv(0),
   lastTimeSend(0),
   waitAttempt(0),
   waitTimer(0),
   orderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memcpy(&addr, &address, SOCKADDR_SIZE);
}

// operations

void NetAddress::removeAcknowledged(uint number)
{
   std::size_t index = 0;
   for ( ; index < resendQueue.size(); ++index )
   {
      ObjectHandle<NetPackage>& package = resendQueue[index];
      if ( package->getNumber() >= number )
      {
         break;
      }
   }

   if ( index < resendQueue.size() )
   {
      resendQueue.erase(resendQueue.begin(), resendQueue.begin() + index);
   }
}