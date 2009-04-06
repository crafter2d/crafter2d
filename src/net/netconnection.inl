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
#include <assert.h>
#include "../defines.h"

/******************************************************
 * NetAddress structure
 */

INLINE
NetAddress::NetAddress():
   packageNumber(1),
   lastPackageNumber(0),
   lastTimeRecv(0),
   lastTimeSend(0),
   orderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memset(&addr, 0, SOCKADDR_SIZE);
   addr.sin_family = AF_INET;
}

INLINE
NetAddress::NetAddress(sockaddr_in adr):
   packageNumber(1),
   lastPackageNumber(0),
   lastTimeRecv(0),
   lastTimeSend(0),
   orderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memcpy(&addr, &adr, SOCKADDR_SIZE);
}

/******************************************************
 * NetConnection structure
 */

INLINE void NetConnection::attachProcess(Process* proc)
{
   process = proc;
}

INLINE void NetConnection::setAccepting(bool a)
{
   accept = a; 
}

INLINE void NetConnection::setClientId(const int client)
{
   clientid = client;
}

INLINE int NetConnection::getClientId()
{ 
   return clientid; 
}

INLINE bool NetConnection::isConnected() 
{ 
   return connected;
}

INLINE void NetConnection::setSendAliveMessages(bool yes) 
{
   mSendAliveMsg = yes; 
}

INLINE 
const NetAddress& NetConnection::resolveClient(int idx) const
{
   ASSERT(idx >= 0 && idx < clients.size())
   return *clients[idx];
}

INLINE
NetAddress& NetConnection::resolveClient(int idx)
{
   ASSERT(idx >= 0 && idx < clients.size())
   return *clients[idx];
}
