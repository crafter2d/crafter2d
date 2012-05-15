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

#include <vector>

#include "netobject.h"
#include "netpackage.h"
#include "sortedpackagelist.h"

class NetStream;
class NetStatistics;
class Process;

const int   MAX_PACKAGE_NUMBER            = 0xAFFFFFFF;
const int   MIN_PACKAGE_NUMBER_DIFFERENCE = 0xAFFFF000;
const int   RESET_DIFFERENCE              = 0x1FFFFFFF;
const float MAX_TIME_BETWEEN_RECV         = 3.0f;
const int   HEADER_SIZE                   = sizeof(uint)*2 + 2;
const int   SOCKADDR_SIZE                 = sizeof(sockaddr_in);
const int   INVALID_CLIENTID              = -1;
const char  ALIVE_MSG_ID                  = 0xF;
const float ALIVE_MSG_INTERVAL            = 1.0f;

typedef std::vector<NetPackage*> PackageQueue;

/// NetAddress
/// \brief Keeps the information about a client or server IP and port.
struct NetAddress {
   NetAddress();
   NetAddress(sockaddr_in adr);

   sockaddr_in addr;

   uint  packageNumber;
   uint  lastPackageNumber;
   float lastTimeRecv;
   float lastTimeSend;

   SortedPackageList orderQueue;
   PackageQueue resendQueue;

   NetStatistics* pstatistics;
};

typedef std::vector<NetAddress*> AdressList;

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

   bool        create(int port=0);
   bool        connect(const char* serverName, int port);
   void        disconnect();
   void        update();

   void        setAccepting(bool a);
   bool        isConnected();

   bool        select (bool read, bool write);

   void        send(const NetStream& stream, NetPackage::Reliability reliability = NetPackage::eReliableSequenced);
   void        send(const NetObject& object, NetPackage::Reliability reliability = NetPackage::eReliableSequenced);
   
   void        recv();

   bool        getMsg(BitStream& stream);

   void        setClientId(const int client);
   int         getClientId();

   void        setSendAliveMessages(bool yes);

   const NetAddress& resolveClient(int idx) const;
         NetAddress& resolveClient(int idx);

protected:
   bool        addNewClient(NetAddress& address);
   int         findClient(const NetAddress& address) const;

   void        send(NetAddress& client, const NetStream& stream, NetPackage::Reliability reliability);
   void        sendAck(NetAddress& client, const NetPackage& package);
   void        sendAliveMessages(float tick);

   void        doSend(NetAddress& client, const NetPackage& package);
   NetPackage* doReceive(NetAddress& address);

   void        resend(NetAddress& client, const NetPackage& package);

   bool        isValidSequencedPackage(const NetAddress& client, const NetPackage& package);

   void        removePackageFromResendQueue(NetAddress& client, uint packageNumber);

   int         getErrorNumber();

private:
   Process&    mProcess;
   AdressList  clients;
   uint        clientid;
   float       lastSendAlive;
   int         sock;
   int         mFlags;
};

#ifdef JENGINE_INLINE
#  include "netconnection.inl"
#endif

#endif
