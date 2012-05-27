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
#include "netconnection.h"
#ifndef JENGINE_INLINE
#  include "netconnection.inl"
#endif

#include <string.h>
#ifdef WIN32
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SOCKET_ERROR -1
#endif

#include "core/smartptr/autoptr.h"
#include "core/streams/arraystream.h"
#include "core/streams/bufferedstream.h"
#include "core/containers/listiterator.h"
#include "core/log/log.h"
#include "core/system/timer.h"

#include "engine/process.h"

#include "events/netevent.h"
#include "netaddress.h"
#include "netstatistics.h"
#include "netobjectstream.h"
#include "netstream.h"

/// \fn NetConnection::initialize()
/// \brief On Windows platforms the Winsock 2 library must be initialized before
/// socket functions can be used.
///
/// Windows platforms use the Winsock 2 library for sockets. Before any socket operation
/// can be performed the library must be initialized. Initialization is done in this
/// function, were also is ensured that version 2.2 is supported.
bool NetConnection::initialize()
{
#ifdef WIN32
   WORD wVersionRequested;
   WSADATA wsaData;
   int err;

   wVersionRequested = MAKEWORD( 2, 2 );
   err = WSAStartup( wVersionRequested, &wsaData );
   if ( err != 0 )
   {
      Log::getInstance().error("Could not initialize WinSock 2.");
      return false;
   }

   if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )
   {
      Log::getInstance().error("WinSock 2.2 is not supported.");
      WSACleanup( );
      return false;
   }
#endif
   return true;
}

/******************************************************
 * NetConnection class
 */

NetConnection::NetConnection(Process& process):
   mProcess(process),
   mClients(),
   mAllocator(),
   mSock(-1),
   mFlags(0)
{
}

NetConnection::~NetConnection()
{
}

/// \fn NetConnection::create(int port)
/// \brief Creates and binds a socket to port
/// \param port [Optional] port number to bind to
///
/// This method creates a socket and optionaly binds it to a port.
bool NetConnection::create(int port)
{
   mSock = (int)socket(AF_INET, SOCK_DGRAM, 0);
   if ( mSock == -1 )
   {
      Log::getInstance().error("Could not create socket.");
      return false;
   }

   // set the socket to non blocking
#ifdef WIN32
   unsigned long non_block = 1;
   ioctlsocket (mSock, FIONBIO, &non_block);
#else
   fcntl (mSock, F_SETFL, O_NONBLOCK );
#endif

   if (port > 0)
   {
      sockaddr_in sa;
      sa.sin_family = AF_INET;
      sa.sin_port = htons(port);
      sa.sin_addr.s_addr = htonl(INADDR_ANY);

      if ( bind (mSock, (sockaddr*)&sa, sizeof(sa)) < 0 )
      {
         Log::getInstance().error("Could not bind socket to port %d.", port);
         return false;
      }

      // server is by default connected when created
      SET_FLAG(mFlags, eConnected);
   }

   return true;
}

/// \fn NetConnection::connect(const std::string& server, int port)
/// \brief Set up this connection for communication with a server.
/// \returns clientid of remote connection
int NetConnection::connect(const std::string& serverName, int port)
{
   sockaddr_in client;
   client.sin_family = AF_INET;
   client.sin_port = 0;
   client.sin_addr.s_addr = htonl(INADDR_ANY);
   if ( bind(mSock, (sockaddr*)&client, sizeof client) < 0 )
   {
      Log::getInstance().error("Can not bind client socket.");
      return false;
   }

   NetAddress address;
   address.addr.sin_family = AF_INET;
	address.addr.sin_port = htons ((u_short)port);
   address.addr.sin_addr.s_addr = inet_addr(serverName.c_str());

	if (address.addr.sin_addr.s_addr == INADDR_NONE)
   {
      hostent *host_info = gethostbyname(serverName.c_str());
		if (host_info == NULL)
			return false;
		memcpy (&address.addr.sin_addr, host_info->h_addr, host_info->h_length);
	}

   int clientid = addNewClient(address);
   SET_FLAG(mFlags, eConnected);
   
   return clientid;
}

/// \fn NetConnection::disconnect()
/// \brief Closes the connection to all connected clients.
void NetConnection::disconnect()
{
   if ( IS_SET(mFlags, eConnected) )
   {
      CLEAR_FLAG(mFlags, eConnected);

      // close the socket connection
#ifdef WIN32
      shutdown(mSock, SD_BOTH);
      closesocket(mSock);
#else
      shutdown(mSock, SHUT_RDWR);
      close(mSock);
#endif

      // remove the clients
      mClients.clear();
   }
}

/// \fn NetConnection::getErrorNumber()
/// \brief Returns the error number when a socket function failed. Currently, this function
/// is only supported on the Windows platform.
NetConnection::SocketError NetConnection::getErrorNumber() const
{
#ifdef WIN32
   SocketError result = eUnsupportedError;
   switch ( WSAGetLastError() )
   {
   case WSAECONNRESET:
      result = eConnReset;
      break;
   default:
      break;
   }
   return result;
#else
   return -1;
#endif
}

void NetConnection::handleError(NetAddress& client, SocketError error)
{
   switch ( error )
   {
   case eConnReset:
   case eConnTimeout:
      // connection closed by the other side/timeout
      break;
   default:
      Log::getInstance().error("NetConnection.handleError : unsupported socket error detected!");
      break;
   }
}

#define JENGINE_AUTODISCONNECT

/// \fn NetConnection::update()
/// \brief Updates the message queues etc.
void NetConnection::update()
{
   ASSERT(IS_SET(mFlags, eConnected));

   Timer& timer = TIMER;
   
   // receive pending messages
   while (select(true, false))
   {
      receive();
   }

   // resend messages if necessary
   int size = mClients.size();
   for( int index = 0; index < size; ++index )
   {
      NetAddress& client = mClients[index];

#ifdef JENGINE_CONNTIMEOUT
      if ((timer.getTick() - client.lastTimeRecv) > MAX_TIME_BETWEEN_RECV)
      {
         // the client has been time out, remove the bastard
         Log::getInstance().error("Player %d has been timed out!", index);
         handleError(client, eConnTimeout);
      }
      else
#endif
      {
         process(index);

         sendAck(client);
         sendAlive(client, timer.getTick());

         if ( client.pstatistics )
         {
            client.pstatistics->update(timer.getTick());
         }
      }
   }
}

void NetConnection::process(int clientid)
{
   NetAddress& client = mClients[clientid];
   if ( client.waitAttempt > 0 )
   {
      client.waitTimer -= TIMER.getTick();
      if ( client.waitTimer > 0.0f )
      {
         return;
      }
   }

   ListIterator<PackageHandle> it(client.orderQueue);
   while ( it.isValid() )
   {
      PackageHandle& handle = *it;
      NetPackage& package = *handle;

      if ( package.getNumber() == client.nextPackageNumber )
      {
         processPackage(clientid, package);
      }
      else if ( package.getNumber() > client.nextPackageNumber )
      {
         // we miss a package, start the wait timer
         client.waitAttempt++;
         client.waitTimer = client.waitAttempt * client.waitAttempt;
         break;
      }

      it.remove();
   }
}

void NetConnection::processPackage(int clientid, NetPackage& package)
{
   NetObject* pobject = NULL;
   ArrayStream arraystream(package.getData(), package.getDataSize());
   NetObjectStream stream(arraystream);
   stream >> &pobject;

   AutoPtr<NetEvent> event(dynamic_cast<NetEvent*>(pobject));
   mProcess.onClientEvent(clientid, *event);

   NetAddress& client = mClients[clientid];
   client.nextPackageNumber++;
}

/// \fn NetConnection::send(int clientid, DataStream& stream, NetPackage::Reliability reliability)
/// \brief Sends the data from the bitstream to the client/server
void NetConnection::send(int clientid, const NetStream& stream, NetPackage::Reliability reliability)
{
   NetAddress& client = mClients[clientid];
   send(client, stream, reliability);
}

/// \fn NetConnection::send(int clientid, NetObject* obj, PacketReliability reliability)
/// \brief Transmits a NetObject to the current client.
void NetConnection::send(int clientid, const NetObject& object, NetPackage::Reliability reliability)
{
   NetAddress& client = mClients[clientid];

   BufferedStream bufferedstream;
   NetObjectStream stream(bufferedstream);
   stream << object;

   send(client, stream, reliability);
}

void NetConnection::send(NetAddress& client, const NetStream& stream, NetPackage::Reliability reliability)
{
   PackageHandle package(mAllocator);
   package->reset();
   package->setType(NetPackage::eEvent);
   package->setReliability(reliability);
   package->setNumber(client.packageNumber);
   package->setData(stream.getDataSize(), stream.getData());

   ASSERT(package->getType() == NetPackage::eEvent);

   // update the package numbering
   if ( ++client.packageNumber > MAX_PACKAGE_NUMBER )
      client.packageNumber = 0;

   doSend(client, *package);

   // when reliability is requested, save the package in the resend queue
   if ( reliability >= NetPackage::eReliableSequenced )
   {
      client.resendQueue.push_back(package);
   }
}

/// \fn NetConnection::sendAck(const NetPackage& package)
/// \brief Send an acknowledgement back to the sender of a reliable package.
void NetConnection::sendAck(NetAddress& client)
{
   NetPackage ackPackage(NetPackage::eAck, NetPackage::eUnreliable, client.nextPackageNumber-1);

   doSend(client, ackPackage);
}

/// \fn NetConnection::sendAlive(NetAddress& client, float tick)
/// \brief Sends an alive system message, when no message has been send for some time.
void NetConnection::sendAlive(NetAddress& client, float tick)
{
   if ( IS_SET(mFlags, eKeepAlive) && (tick - client.lastTimeSend >= ALIVE_MSG_INTERVAL) )
   {
      NetPackage package(NetPackage::eAlive, NetPackage::eUnreliable, 0);
      doSend(client, package);
   }
}

void NetConnection::doSend(NetAddress& client, const NetPackage& package)
{
   int size = package.getSize();
   int send = sendto(mSock, (const char*)&package, size, 0, (struct sockaddr*)&(client.addr), NetAddress::SOCKADDR_SIZE);
   if ( send == SOCKET_ERROR )
   {
      handleError(client, getErrorNumber());
   }
   else
   {
      client.lastTimeSend = TIMER.getTick();
      if ( client.pstatistics )
      {
         client.pstatistics->addPackageSend(send);
      }
   }
}

/// \fn NetConnection::receive()
/// \brief Receive data from the socket
void NetConnection::receive()
{
   NetAddress address;
   PackageHandle package(doReceive(address));
   if ( package.hasObject() )
   {
      // see if the connection is already in the list
      int clientid = findOrCreate(address); 
      if ( clientid != INVALID_CLIENTID )
      {
         NetAddress& client = mClients[clientid];
         client.lastTimeRecv = Timer::getInstance().getTick();

         if ( client.pstatistics )
         {
            client.pstatistics->addPackageSend(package->getDataSize());
         }

         switch ( package->getType() )
         {
            case NetPackage::eAck:
               {
                  client.removeAcknowledged(package->getNumber());
                  break;
               }
            case NetPackage::eAlive:
               {
                  // don't do anything with the I'm alive message
                  break;
               }
            case NetPackage::eEvent:
               {
                  NetPackage::Reliability reliability = package->getReliability();
                  if ( reliability == NetPackage::eReliableSequenced )
                  {
                     // the older packages are no longer valid, can be removed
                     client.orderQueue.removeOldPackages(package->getNumber());
                     client.nextPackageNumber = package->getNumber();
                  }

                  client.orderQueue.add(package);
                  break;
               }
            case NetPackage::eInvalid:
            default:
               UNREACHABLE("Invalid package type.");
               break;
         }
      }
   }
}

PackageHandle NetConnection::doReceive(NetAddress& address)
{
   PackageHandle package(mAllocator);
   int addrLen = NetAddress::SOCKADDR_SIZE;

   int size = recvfrom(mSock, (char*)package.ptr(), NetPackage::MaxPackageSize, 0, (struct sockaddr*)&(address.addr), (socklen_t*)&addrLen);
   if ( size == SOCKET_ERROR )
   {
      handleError(address, getErrorNumber());
      return PackageHandle();
   }

   return package;
}

/// \fn NetConnection::select(bool read, bool write)
/// \brief Checks if the socket is ready for reading/writting
bool NetConnection::select(bool read, bool write)
{
   fd_set readSet;
   struct timeval timeVal;

   FD_ZERO(&readSet);
   FD_SET(mSock, &readSet);
   timeVal.tv_sec = 0;
   timeVal.tv_usec = 0;

   // poll (non-blocking) if there is data waiting
   return (::select(mSock+1, &readSet, NULL, NULL, &timeVal) > 0);
}

/// \fn NetConnection::addNewClient(const NetAddress& address)
/// \brief Add new client to the client list.
/// \param address Address of the new client thats connected
/// \returns the clientid of the new connection
int NetConnection::addNewClient(const NetAddress& address)
{
   NetAddress* paddr = new NetAddress(address.addr);
   paddr->lastTimeRecv = Timer::getInstance().getTick();
   // paddr->pstatistics = new NetStatistics();

   return mClients.add(paddr);
}

/// \fn NetConnection::findOrCreate(const NetAddress& client)
/// \brief Finds or creates an address based on the given client info
/// \return the clientid of the client or INVALID_CLIENTID when not found (e.g. when not accepting)
int NetConnection::findOrCreate(const NetAddress& client)
{
   int clientid = mClients.indexOf(client);
   if ( clientid == INVALID_CLIENTID )
   {
      if ( IS_SET(mFlags, eAccept) )
      {
         clientid = addNewClient(client);
      }
   }
   return clientid;
}
