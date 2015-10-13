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

#include <cstring>
#include <algorithm>

INLINE
NetAddress::NetAddress():
   index(0),
   packageNumber(1),
   nextPackageNumber(1),
   lastTimeRecv(0),
   lastTimeSend(0),
   waitAttempt(0),
   waitTimer(0),
   mOrderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memset(&addr, 0, SOCKADDR_SIZE);
   addr.sin_family = AF_INET;
}

INLINE
NetAddress::NetAddress(const sockaddr_in& address):
   index(0),
   packageNumber(1),
   nextPackageNumber(1),
   lastTimeRecv(0),
   lastTimeSend(0),
   waitAttempt(0),
   waitTimer(0),
   mOrderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memcpy(&addr, &address, SOCKADDR_SIZE);
}

// operations

void NetAddress::removeAcknowledged(uint32_t number)
{
   resendQueue.erase(
      std::remove_if(resendQueue.begin(), resendQueue.end(), [&number](const PackageHandle& p)
      {
         if (p->getNumber() < number)
         {
            return true;
         }
         return false;
      }), resendQueue.end());
}
