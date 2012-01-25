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

#include "core/smartptr/autoptr.h"
#include "core/log/log.h"
#include "core/system/timer.h"

#include "engine/process.h"

#include "events/connectreplyevent.h"

#include "netstatistics.h"

#ifdef WIN32
#include <ws2tcpip.h>
#else
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SOCKET_ERROR -1
#endif

/******************************************************
 * NetConnection class
 */

NetConnection::NetConnection(Process& process):
   mProcess(process),
   clients(),
   lastSendAlive(0),
   clientid(0),
   sock(-1),
   mSendAliveMsg(false),
   connected(false),
   accept(true)
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
   sock = (int)socket(AF_INET, SOCK_DGRAM, 0);
   if ( sock == -1 )
   {
      Log::getInstance().error("Could not create socket.");
      return false;
   }

   // set the socket to non blocking
#ifdef WIN32
   unsigned long non_block = 1;
   ioctlsocket (sock, FIONBIO, &non_block);
#else
   fcntl (sock, F_SETFL, O_NONBLOCK );
#endif

   if (port > 0)
   {
      sockaddr_in sa;
      sa.sin_family = AF_INET;
      sa.sin_port = htons(port);
      sa.sin_addr.s_addr = htonl(INADDR_ANY);

      if ( bind (sock, (sockaddr*)&sa, sizeof(sa)) < 0 ) 
      {
         Log::getInstance().error("Could not bind socket to port %d.", port);
         return false;
      }

      // server is by default connected when created
      connected = true;
   }

   lastSendAlive = TIMER.getTick();
   return true;
}

/// \fn NetConnection::connect(const char* server, int port)
/// \brief Set up this connection for communication with a server.
bool NetConnection::connect(const char* serverName, int port)
{
   sockaddr_in client;
   client.sin_family = AF_INET;
   client.sin_port = 0;
   client.sin_addr.s_addr = htonl(INADDR_ANY);
   if ( bind(sock, (sockaddr*)&client, sizeof client) < 0 )
   {
      Log::getInstance().error("Can not bind client socket.");
      return false;
   }

   NetAddress address;
   address.addr.sin_family = AF_INET;
	address.addr.sin_port = htons ((u_short)port);
	address.addr.sin_addr.s_addr = inet_addr(serverName);

	if (address.addr.sin_addr.s_addr == INADDR_NONE)
   {
		hostent *host_info = gethostbyname (serverName);
		if (host_info == NULL)
			return false;
		memcpy (&address.addr.sin_addr, host_info->h_addr, host_info->h_length);
	}

   addNewClient(address);
   connected = true;
   return true;
}

void NetConnection::disconnect()
{
   if ( connected )
   {
      connected = false;

      // close the socket connection
#ifdef WIN32
      shutdown(sock, SD_BOTH);
      closesocket(sock);
#else
      shutdown(sock, SHUT_RDWR);
      close(sock);
#endif

      // remove the clients
      for ( int i=0; i < clients.size(); ++i )
      {
         delete clients[i];
      }
      clients.clear();
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
   Timer& timer = TIMER;

   if ( mSendAliveMsg )
   {
      sendAliveMessages(timer.getTick());
   }

   // receive pending messages
   while (select(true, false))
   {
      recv();
   }

   // resend messages if necessary
   AdressList::iterator it = clients.begin();
   for(int i = 0; it != clients.end(); ++it, ++i) 
   {
      NetAddress* pclient = (*it);

      if ( pclient->pstatistics )
      {
         pclient->pstatistics->update(timer.getTick());
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
         PackageQueue::iterator pit = pclient->resendQueue.begin();
         for (; pit != pclient->resendQueue.end(); ++pit) 
         {
            NetPackage& package = *(*pit);
            const float tick = timer.getTick();

            if ( tick - package.getTimeStamp() > MAX_TIME_BETWEEN_RECV)
            {
               package.setTimeStamp(tick);

               Log::getInstance().info("Resending package number for client %d: %d", i, package.getNumber());
               resend(*pclient, package);
            }
         }
      }
   }
}

void NetConnection::send(NetAddress& client, BitStream* stream, NetPackage::Reliability reliability)
{
   AutoPtr<NetPackage> package = new NetPackage(NetPackage::eEvent, reliability, client.packageNumber, stream->getSize(), stream->getBuf());

   // update the package numbering
   if (++client.packageNumber > MAX_PACKAGE_NUMBER)
      client.packageNumber = 0;

   BitStream packageStream;
   *package >> packageStream;
   doSend(client, packageStream);

   // when reliability is requested, save the package in the resend queue
   if ( reliability >= NetPackage::eReliableSequenced )
   {
      client.resendQueue.push_back(package.release());
   }
}

/// \fn NetConnection::send(BitStream& stream)
/// \brief Sends the data from the bitstream to the client/server
void NetConnection::send(BitStream* stream, NetPackage::Reliability reliability)
{
   NetAddress& client = *clients[clientid];
   send(client, stream, reliability);
}

/// \fn NetConnection::send(NetObject* obj, PacketReliability reliability)
/// \brief Transmits a NetObject to the current client.
void NetConnection::send(NetObject* obj, NetPackage::Reliability reliability)
{
   BitStream stream;
   stream << obj;
   send(&stream, reliability);
}

void NetConnection::resend(NetAddress& client, const NetPackage& package)
{
   BitStream stream;
   package >> stream;

   // resend the package to the client
   doSend(client, stream);
}

void NetConnection::doSend(NetAddress& client, const BitStream& stream)
{
   int err = sendto (sock, stream.getBuf(), stream.getSize(), 0, (struct sockaddr*)&(client.addr), SOCKADDR_SIZE);
   if (err == SOCKET_ERROR)
   {
      Log::getInstance().error("NetConnection.resend : error during sending(%d)", getErrorNumber());
   }
   else
   {
      client.lastTimeSend = Timer::getInstance().getTick();
      if ( client.pstatistics )
      {
         client.pstatistics->addPackageSend(stream.getSize());
      }
   }
}

void NetConnection::sendAliveMessages(float tick)
{
   BitStream stream;
   NetPackage package(NetPackage::eAlive, NetPackage::eUnreliable, 0);
   package >> stream;

   for ( int i = 0; i < clients.size(); ++i )
   {
      NetAddress& client = *clients[i];

      if ( tick - client.lastTimeSend > 1.0f )
      {
		   doSend(client, stream);
	   }
   }
}

/// \fn NetConnection::recv(BitStream& stream)
/// \brief Receive data from the socket
void NetConnection::recv()
{
   BitStream recvStream;
   NetAddress address;

   if ( !doReceive(address, recvStream) )
   {
      return;
   }

   // see if the connection is already in the list
   if ((clientid = findClient(address)) == -1)
   {
      if (!addNewClient(address))
         return;
   }

   NetAddress& client = resolveClient(clientid);
   client.lastTimeRecv = Timer::getInstance().getTick();
   if ( client.pstatistics )
   {
      client.pstatistics->addPackageSend(recvStream.getSize());
   }

   // convert package into a header and stream
   NetPackage package;
   package << recvStream;

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
            insertOrderedPackage(client, package);

            // check if we have already the package we are waiting for
            PackageList::iterator it = client.orderQueue.begin();
            NetPackage& pack = (*it);
            if ( pack.getNumber() == client.lastPackageNumber )
            {
               // erase the package from the list
               client.orderQueue.erase(it);
            }
            else
            {
               // nope, still waiting for the right one
               return;
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

      AutoPtr<NetObject> event = package.getObject();
      mProcess.onClientEvent(clientid, dynamic_cast<NetEvent&>(*event));
   }
}

bool NetConnection::doReceive(NetAddress& address, BitStream& recvStream)
{
   int addrLen = SOCKADDR_SIZE;
   char buffer[MAX_BITSTREAM_BUFSIZE];

   int size = recvfrom(sock, buffer, MAX_BITSTREAM_BUFSIZE, 0, (struct sockaddr*)&address.addr, (socklen_t*)&addrLen);
   if ( size == SOCKET_ERROR )
   {
      Log::getInstance().error("An error occured while receiving a package (%d)", getErrorNumber());
      return false;
   }
   else
   {
      recvStream.setBuffer(buffer, size);
      return true;
   }
}

/// \fn NetConnection::select(bool read, bool write)
/// \brief Checks if the socket is ready for reading/writting
bool NetConnection::select(bool read, bool write)
{
   fd_set readSet;
   struct timeval timeVal;

   FD_ZERO(&readSet);
   FD_SET(sock, &readSet);
   timeVal.tv_sec = 0;
   timeVal.tv_usec = 0;

   // poll (non-blocking) if there is data waiting
   return (::select(sock+1, &readSet, NULL, NULL, &timeVal) > 0);
}

/// \fn NetConnection::addNewClient(const NetAddress& address)
/// \brief Add new client to the client list.
/// \param address Address of the new client thats connected
bool NetConnection::addNewClient(NetAddress& address)
{
   // see if the process wants to accept a connection from this client
   if ( accept )
   {
      int reason = mProcess.allowNewConnection();
      if ( reason != 0 )
      {
         ConnectReplyEvent event(ConnectReplyEvent::eDenite, reason);

         BitStream stream;
         stream << &event;

         send(address, &stream, NetPackage::eUnreliable);
         return false;
      }

      // build the new address structure
      NetAddress* addr = new NetAddress(address.addr);
      addr->lastTimeRecv = Timer::getInstance().getTick();

      // create new statistics
      // addr->pstatistics = new NetStatistics();

      int i = 0;
      for (; i < clients.size(); ++i)
      {
         if (clients[i] == NULL)
            break;
      }

      // insert it in the list
      if (i < clients.size())
      {
         clients[i] = addr;
         clientid = i;
      }
      else 
      {
         clients.push_back(addr);
         clientid = clients.size()-1;
      }

      return true;
   }
   else
      return false;
}

/// \fn NetConnection::findClient(const NetAddress& address)
/// \brief Searches for the client id, given a network address structure.
int NetConnection::findClient(const NetAddress& address) const
{
   for ( int i = 0 ; i < clients.size(); ++i )
   {
      const NetAddress& client = resolveClient(i);
#ifdef WIN32
      if (client.addr.sin_addr.S_un.S_addr == address.addr.sin_addr.S_un.S_addr)
#else
      if (client.addr.sin_addr.s_addr == address.addr.sin_addr.s_addr)
#endif
      {
         return i;
      }
   }

   return -1;
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
   BitStream stream;
   NetPackage ackPackage(NetPackage::eAck, NetPackage::eUnreliable, package.getNumber());
   ackPackage >> stream;

   doSend(client, stream);
}

/// \fn NetConnection::removePackageFromResendQueue(NetAddress& client, int packageNumber)
/// \brief Removes an acknowledged package from the resend queue.
void NetConnection::removePackageFromResendQueue(NetAddress& client, uint packageNumber)
{
   PackageQueue::iterator it = client.resendQueue.begin();
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

/// \fn NetConnection::insertOrderedPackage(NetAddress& client, const NetPackage& package)
/// \brief Inserts a package in the order list for a client. We assume that the clientid 
/// contains the right index to the client for which this package was meant
void NetConnection::insertOrderedPackage(NetAddress& client, const NetPackage& package)
{
   if (client.orderQueue.size() > 0)
   {
      // find the correct location to put in the package
      PackageList::iterator it = client.orderQueue.begin();
      while (it != client.orderQueue.end() && (*it).getNumber() < package.getNumber())
      {
         ++it;
      }

      NetPackage& savedpackage = *it;

      if ( it != client.orderQueue.end() )
      {
         // note that eventually the package could already been there (if for example
         // the ACK got lost), so make sure the new package number is smaller
         if ( package.getNumber() < savedpackage.getNumber() )
         {
            client.orderQueue.insert(it, package);
         }
      }
      else
         // stupid message is way earlier than the rest! push it at the end of the list
         client.orderQueue.push_back(package);
   }
   else
   {
      // first out of order package, just push in in the vector
      client.orderQueue.push_back(package);
   }
}
