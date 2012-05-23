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

#include "netclients.h"
#include "netpackage.h"
#include "sortedpackagelist.h"

class NetAddress;
class NetStream;
class Process;

const int   MAX_PACKAGE_NUMBER            = 0xAFFFFFFF;
const int   MIN_PACKAGE_NUMBER_DIFFERENCE = 0xAFFFF000;
const int   RESET_DIFFERENCE              = 0x1FFFFFFF;
const float MAX_TIME_BETWEEN_RECV         = 3.0f;
const int   HEADER_SIZE                   = sizeof(uint)*2 + 2;
const int   INVALID_CLIENTID              = -1;
const char  ALIVE_MSG_ID                  = 0xF;
const float ALIVE_MSG_INTERVAL            = 1.0f;

/// NetConnection
/// Handles the socket connections for the server and the clients. A NetConnection instance
/// can handle one or more simultanious connections with other NetConnection instances via
/// both Internet and a local network.
class ENGINE_API NetConnection
{
public:
   enum Flags
   {
      eConnected = 1,
      eAccept    = 2,
      eKeepAlive = 4
   };

   explicit    NetConnection(Process& process);
               ~NetConnection();

#ifdef WIN32
   static bool initialize();
#endif

 // query
   void        setAccepting(bool a);
   void        setSendAliveMessages(bool yes);

   bool        isConnected();

 // operations
   bool        create(int port=0);
   int         connect(const std::string& serverName, int port);
   void        disconnect();
   void        update();

 // sending
   void        send(int clientid, const NetStream& stream, NetPackage::Reliability reliability = NetPackage::eReliableSequenced);
   void        send(int clientid, const NetObject& object, NetPackage::Reliability reliability = NetPackage::eReliableSequenced);

private:
 // query   
   int         getErrorNumber();
   bool        isValidSequencedPackage(const NetAddress& client, const NetPackage& package);

 // operations
   int         addNewClient(NetAddress& address);

   bool        select (bool read, bool write);
   void        recv();

   void        send(NetAddress& client, const NetStream& stream, NetPackage::Reliability reliability);
   void        sendAck(NetAddress& client, const NetPackage& package);
   void        sendAliveMessages(float tick);
   void        resend(NetAddress& client, const NetPackage& package);

   void        doSend(NetAddress& client, const NetPackage& package);
   NetPackage* doReceive(NetAddress& address);
   
   void        removePackageFromResendQueue(NetAddress& client, uint packageNumber);

 // members
   Process&    mProcess;
   NetClients  mClients;
   float       mLastSendAlive;
   int         mSock;
   int         mFlags;
};

#ifdef JENGINE_INLINE
#  include "netconnection.inl"
#endif

#endif
