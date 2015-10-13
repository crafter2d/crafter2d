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
#ifndef _NETCONNECTION_H_
#define _NETCONNECTION_H_

#include "engine/engine_base.h"

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "core/memory/objectallocator.h"
#include "core/memory/objecthandle.h"

#include "netclients.h"
#include "netpackage.h"
#include "netsocket.h"

class NetAddress;
class NetObserver;
class NetStream;
class String;

const int   MAX_PACKAGE_NUMBER            = 0xAFFFFFFF;
const int   INVALID_CLIENTID              = -1;
const float ALIVE_MSG_INTERVAL            = 2.0f;
const float WAIT_INTERVAL                 = 0.2f;

/// NetConnection
/// Handles the socket connections for the server and the clients. A NetConnection instance
/// can handle one or more simultanious connections with other NetConnection instances via
/// both Internet and a local network.
class ENGINE_API NetConnection
{
   typedef ObjectAllocator<NetPackage> PackageAllocator;

public:
   enum Flags
   {
      eConnected = 1,
      eAccept    = 2,
      eKeepAlive = 4
   };

   explicit    NetConnection(NetObserver& observer);
               ~NetConnection();

   static bool initialize();

 // query
   void        setAccepting(bool a);
   void        setSendAliveMessages(bool yes);

   bool        isConnected();

 // operations
   bool        listen(int port);
   int         connect(const String& serverName, int port);
   void        shutdown();
   void        disconnect(int client);
   void        update();

 // sending
   void        send(int clientid, const NetStream& stream, NetPackage::Reliability reliability = NetPackage::eReliableOrdered);
   void        send(int clientid, const NetObject& object, NetPackage::Reliability reliability = NetPackage::eReliableOrdered);

private:
 // operations
   NetAddress& addNewClient(const NetAddress& address);
   NetAddress& findOrCreate(const NetAddress& address);

   void        process(NetAddress& client);
   void        processPackage(NetAddress& client, const NetPackage& package);

   void        receive();

   void        send(NetAddress& client, const NetStream& stream, NetPackage::Reliability reliability);
   void        sendAck(NetAddress& client);
   void        sendRequest(NetAddress& client);
   void        sendAlive(NetAddress& client, float tick);
   void        send(NetAddress& client, const NetPackage& package);
   
 // members
   NetObserver&      mObserver;
   NetClients        mClients;
   PackageAllocator  mAllocator;
   NetSocket         mSocket;
   int               mFlags;
};

#ifdef JENGINE_INLINE
#  include "netconnection.inl"
#endif

#endif
