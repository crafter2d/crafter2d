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
#include "core/log/log.h"
#include "core/system/timer.h"

#include "engine/process.h"

#include "events/netevent.h"
#include "netaddress.h"
#include "netstatistics.h"
#include "netobjectstream.h"
#include "netstream.h"

/******************************************************
 * NetConnection class
 */

NetConnection::NetConnection(Process& process):
   mProcess(process),
   mClients(),
   mLastSendAlive(0),
   mSock(-1),
   mFlags(0)
{
}

NetConnection::~NetConnection()
{
}

#ifdef WIN32
/// \fn NetConnection::initialize()
/// \brief On Windows platforms the Winsock 2 library must be initialized before
/// socket functions can be used.
///
/// Windows platforms use the Winsock 2 library for sockets. Before any socket operation
/// can be performed the library must be initialized. Initialization is done in this
/// function, were also is ensured that version 2.2 is supported.
bool NetConnection::initialize()
{
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
   return true;
}
#endif

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

   mLastSendAlive = TIMER.getTick();
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
   if ( clientid != INVALID_CLIENTID )
   {
      SET_FLAG(mFlags, eConnected);
   }

   return clientid;
}

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
int NetConnection::getErrorNumber()
{
#ifdef WIN32
   return WSAGetLastError();
#else
   return -1;
#endif
}

/// \fn NetConnection::update()
/// \brief Updates the message queues etc.
void NetConnection::update()
{
   ASSERT(IS_SET(mFlags, eConnected));

   Timer& timer = TIMER;

   if ( IS_SET(mFlags, eKeepAlive) )
   {
      sendAliveMessages(timer.getTick());
   }

   // receive pending messages
   while (select(true, false))
   {
      recv();
   }

   // resend messages if necessary
   int size = mClients.size();
   for( int index = 0; index < size; ++index )
   {
      NetAddress& client = mClients[index];

      if ( client.pstatistics )
      {
         client.pstatistics->update(timer.getTick());
      }

#ifdef JENGINE_AUTODISCONNECT
      // check if the client has disconnected/timed out
      if ((SDL_GetTicks() - client.lastTimeRecv) > MAX_TIME_BETWEEN_RECV)
      {
         // the client has been time out, remove the bastard
         Console::getInstance().printf("Player %d has been timed out!", i);
         delete pclient;
         pclient = NULL;
      }
      else
#endif
      {
         NetAddress::PackageQueue::iterator pit = client.resendQueue.begin();
         for ( ; pit != client.resendQueue.end(); ++pit )
         {
            NetPackage& package = *(*pit);
            const float tick = timer.getTick();

            if ( tick - package.getTimeStamp() > MAX_TIME_BETWEEN_RECV)
            {
               package.setTimeStamp(tick);

               Log::getInstance().info("Resending package number for client %d: %d", index, package.getNumber());
               resend(client, package);
            }
         }
      }
   }
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
   AutoPtr<NetPackage> package = new NetPackage(NetPackage::eEvent, reliability, client.packageNumber, stream.getDataSize(), stream.getData());

   // update the package numbering
   if (++client.packageNumber > MAX_PACKAGE_NUMBER)
      client.packageNumber = 0;

   doSend(client, *package);

   // when reliability is requested, save the package in the resend queue
   if ( reliability >= NetPackage::eReliableSequenced )
   {
      client.resendQueue.push_back(package.release());
   }
}

void NetConnection::resend(NetAddress& client, const NetPackage& package)
{
   doSend(client, package);
}

void NetConnection::doSend(NetAddress& client, const NetPackage& package) //const BitStream& stream)
{
   int size = package.getSize();
   int err = sendto(mSock, (const char*)&package, size, 0, (struct sockaddr*)&(client.addr), SOCKADDR_SIZE);
   if (err == SOCKET_ERROR)
   {
      Log::getInstance().error("NetConnection.resend : error during sending(%d)", getErrorNumber());
   }
   else
   {
      client.lastTimeSend = Timer::getInstance().getTick();
      if ( client.pstatistics )
      {
         client.pstatistics->addPackageSend(package.getDataSize());
      }
   }
}

void NetConnection::sendAliveMessages(float tick)
{
   NetPackage package(NetPackage::eAlive, NetPackage::eUnreliable, 0);

   for ( int i = 0; i < mClients.size(); ++i )
   {
      NetAddress& client = mClients[i];

      if ( tick - client.lastTimeSend > 1.0f )
      {
		   doSend(client, package);
	   }
   }
}

/// \fn NetConnection::recv(BitStream& stream)
/// \brief Receive data from the socket
void NetConnection::recv()
{
   NetAddress address;
   AutoPtr<NetPackage> ppackage = doReceive(address);
   if ( !ppackage.hasPointer() )
   {
      return;
   }

   NetPackage& package = *ppackage;

   // see if the connection is already in the list
   int clientid = mClients.indexOf(address);
   if ( clientid == -1 )
   {
      if ( IS_SET(mFlags, eAccept) )
      {
         clientid = addNewClient(address);
      }
      else
      {
         return;
      }
   }

   NetAddress& client = mClients[clientid];
   client.lastTimeRecv = Timer::getInstance().getTick();
   if ( client.pstatistics )
   {
      client.pstatistics->addPackageSend(package.getDataSize());
   }

   if ( package.getType() == NetPackage::eAck )
   {
      // remove package from the resend list and return
      removePackageFromResendQueue(client, package.getNumber());
   }
   else if ( package.getType() == NetPackage::eAlive )
   {
      // don't do anything with the I'm alive message
   }
   else
   {
      NetPackage::Reliability reliability = package.getReliability();

      // if the message is reliable, send an ack back
      if ( reliability >= NetPackage::eReliableSequenced )
         sendAck(client, package);

      switch ( reliability )
      {
      case NetPackage::eUnreliable:
         // the package can be used always
         break;
      case NetPackage::eUnreliableSequenced:
      case NetPackage::eReliableSequenced:
         // if this package has an old package number, then drop it
         if ( !isValidSequencedPackage(client, package) )
         {
            return;
         }
         break;
      case NetPackage::eReliableOrdered:
         // make sure client receives the messages in order (+1 as
         // the new package is at least 1 bigger than the last received one).
         if ( false && package.getNumber() > (client.lastPackageNumber+1) )
         {
            client.orderQueue.add(ppackage.release());

            // check if we have already the package we are waiting for
            ListIterator<NetPackage> it(client.orderQueue);
            if ( it.isValid() )
            {
               NetPackage& firstpackage = *it;
               if ( firstpackage.getNumber() == client.lastPackageNumber )
               {
                  it.remove();
               }
               else
               {
                  // nope, still waiting for the right one
                  return;
               }
            }
         }
         break;
      default:
         //assert(false && "Should not get here!");
         UNREACHABLE("Should not get here!")
      }

      // increase the last received package number only if the package number
      // is the next one. sequenced packages which should be received later
      // will be stored in the list. this will result in an orderly fasion
      // of processing the messages.
      client.lastPackageNumber = package.getNumber();

      NetObject* pobject = NULL;
      ArrayStream arraystream(package.getData(), package.getDataSize());
      NetObjectStream stream(arraystream);
      stream >> &pobject;

      AutoPtr<NetEvent> event(dynamic_cast<NetEvent*>(pobject));
      if ( !mProcess.onClientEvent(clientid, *event) )
      {
         mClients.remove(clientid);
      }
   }
}

NetPackage* NetConnection::doReceive(NetAddress& address)
{
   AutoPtr<NetPackage> ppackage = new NetPackage();
   int addrLen = SOCKADDR_SIZE;

   int size = recvfrom(mSock, (char*)ppackage.getPointer(), NetPackage::MaxPackageSize, 0, (struct sockaddr*)&(address.addr), (socklen_t*)&addrLen);
   if ( size == SOCKET_ERROR )
   {
      Log::getInstance().error("An error occured while receiving a package (%d)", getErrorNumber());
      return NULL;
   }

   return ppackage.release();
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
int NetConnection::addNewClient(NetAddress& address)
{
   NetAddress* paddr = new NetAddress(address.addr);
   paddr->lastTimeRecv = Timer::getInstance().getTick();
   // paddr->pstatistics = new NetStatistics();

   return mClients.add(paddr);
}

bool NetConnection::isValidSequencedPackage(const NetAddress& client, const NetPackage& package)
{
   if ( package.getNumber() > client.lastPackageNumber || client.lastPackageNumber - package.getNumber() > MIN_PACKAGE_NUMBER_DIFFERENCE )
      return true;

   // say max = 250, client.lastPN = 249 and package.nr = 2 -> diff = 247 and thus must be overflow -> set flag
   // then package.nr = 250 -> overflow flag is still set, thus discard the package

   return false;
}

/// \fn NetConnection::sendAck(const NetPackage& package)
/// \brief Send an acknowledgement back to the sender of a reliable package.
void NetConnection::sendAck(NetAddress& client, const NetPackage& package)
{
   NetPackage ackPackage(NetPackage::eAck, NetPackage::eUnreliable, package.getNumber());

   doSend(client, ackPackage);
}

/// \fn NetConnection::removePackageFromResendQueue(NetAddress& client, int packageNumber)
/// \brief Removes an acknowledged package from the resend queue.
void NetConnection::removePackageFromResendQueue(NetAddress& client, uint packageNumber)
{
   NetAddress::PackageQueue::iterator it = client.resendQueue.begin();
   for ( ; it != client.resendQueue.end(); ++it)
   {
      NetPackage* ppackage = *it;
      if ( ppackage->getNumber() == packageNumber)
      {
         client.resendQueue.erase(it);
         delete ppackage;
         return;
      }
   }

   if (packageNumber > client.lastPackageNumber)
   {
      // this is not possible!
      Log::getInstance().error("Received an ACK with an invalid package number (max = %d)!", client.lastPackageNumber);
   }
   else
   {
      // if we get here the package wasn't found!
      Log::getInstance().error("Received an ACK for a non-existing package!");
   }

   Log::getInstance().info("Received package number: %d", packageNumber);
}

