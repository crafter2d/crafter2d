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

#include "../system/timer.h"

#include "../console.h"
#include "../process.h"
#include "../script.h"
#include "../scriptmanager.h"

#include "netevent.h"
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

NetConnection::NetConnection(void):
   mSendAliveMsg(false),
   connected(false),
   lastSendAlive(0),
   clientid(0),
   sock(-1),
   process(0),
   accept(true)
{
}

NetConnection::~NetConnection(void)
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
   if (err != 0) {
      Console::getInstance().print("Could not initialize WinSock 2.");
      return false;
   }

   if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {
      Console::getInstance().print("WinSock 2.2 is not supported.");
      WSACleanup( );
      return false;
   }
   return true;
}
#endif

/// \fn NetConnection::create()
/// \brief Creates and binds a socket to port
/// \param port [Optional] port number to bind to
///
/// This method creates a socket and optionaly binds it to a port.
bool NetConnection::create(Uint32 port)
{
   sock = (int)socket(AF_INET, SOCK_DGRAM, 0);
   if (sock == -1) {
      Console::getInstance().print("Could not create socket.");
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

      if (bind (sock, (sockaddr*)&sa, sizeof(sa)) < 0) {
         Console::getInstance().printf("Could not bind socket to port %d.", port);
         return false;
      }

      // server is by default connected when created
      connected = true;
   }

   lastSendAlive = SDL_GetTicks();
   return true;
}

/// \fn NetConnection::connect(const char* server, Uint32 port)
/// \brief Set up this connection for communication with a server.
bool NetConnection::connect(const char* serverName, Uint32 port)
{
   sockaddr_in client;
   client.sin_family = AF_INET;
   client.sin_port = 0;
   client.sin_addr.s_addr = htonl(INADDR_ANY);
   if (bind(sock, (sockaddr*)&client, sizeof client) < 0) {
      Console::getInstance().print("Can not bind client socket.");
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

   addNewClient(address, true);
   connected = true;
   return true;
}

void NetConnection::disconnect()
{
   if (connected) {
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
      for ( Uint32 i=0; i < clients.size(); ++i )
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
   Timer& timer = Timer::getInstance();

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
            NetPackage& package = (*pit);
            const float tick = timer.getTick();

            if ( tick - package.time > MAX_TIME_BETWEEN_RECV)
            {
               package.time = tick;

               Console::getInstance().printf("Resending package number for client %d: %d", i, package.number);
               resend(*pclient, package);
            }
         }
      }
   }
}

void NetConnection::send(NetAddress& client, BitStream* stream, PacketReliability reliability)
{
   NetPackage package;
   package.type = reliability;
   package.isAck = false;
   package.clientid = clientid;
   package.number = client.packageNumber;
   package.time = Timer::getInstance().getTick();
   package.datasize = stream->getSize();
   package.data = new char[package.datasize];
   memcpy(package.data, stream->getBuf(), package.datasize);

   // update the package numbering
   if (++client.packageNumber > MAX_PACKAGE_NUMBER)
      client.packageNumber = 0;

   BitStream packageStream;
   convertPackageToBitStream(package, packageStream);
   doSend(client, packageStream);

   // when reliability is requested, save the package in the resend queue
   if (reliability >= Reliable)
   {
      client.resendQueue.push_back (package);
   }
   else
   {
      delete[] package.data;
   }
}

/// \fn NetConnection::send(BitStream& stream)
/// \brief Sends the data from the bitstream to the client/server
void NetConnection::send(BitStream* stream, PacketReliability reliability)
{
   NetAddress& client = *clients[clientid];
   send(client, stream, reliability);
}

/// \fn NetConnection::send(NetObject* obj, PacketReliability reliability)
/// \brief Transmits a NetObject to the current client.
void NetConnection::send(NetObject* obj, PacketReliability reliability)
{
   BitStream stream;
   stream << obj->getRuntimeInfo()->getName().c_str() << obj;
   send(&stream, reliability);
}

void NetConnection::resend(NetAddress& client, const NetPackage& package)
{
   BitStream stream;

   // resend the package to the client
   convertPackageToBitStream(package, stream);
   doSend(client, stream);
}

void NetConnection::doSend(NetAddress& client, const BitStream& stream)
{
   int err = sendto (sock, stream.getBuf(), stream.getSize(), 0, (struct sockaddr*)&(client.addr), SOCKADDR_SIZE);
   if (err == SOCKET_ERROR)
   {
      Console::getInstance().printf ("NetConnection.resend : error during sending(%d)", getErrorNumber());
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
   for ( Uint32 i = 0; i < clients.size(); ++i )
   {
      NetAddress& client = *clients[clientid];

      if ( tick - client.lastTimeSend > 1.0f )
      {
		   NetPackage ackPackage;
		   ackPackage.type = ALIVE_MSG_ID;
		   ackPackage.isAck = false;
		   ackPackage.number = 0;
         ackPackage.time = tick;
		   ackPackage.datasize = 0;

         BitStream stream;
		   convertPackageToBitStream(ackPackage, stream);
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
   convertBitStreamToPackage(recvStream, package);
   if (package.isAck == true)
   {
      // remove package from the resend list and return
      removePackageFromResendQueue(client, package.number);
   }
   else if (package.type == ALIVE_MSG_ID)
   {
      // don't do anything with the I'm alive message
   }
   else
   {
      // if the message is reliable, send an ack back
      if (package.type >= Reliable)
         sendAck(client, package);

      BitStream sendStream;
      switch ( package.type )
      {
      case Unreliable:
         sendStream.setBuffer(package.data, package.datasize);
         break;
      case UnreliableSequenced:
      case ReliableSequenced:
         // if this package has an old package number, then drop it
         if ( isValidSequencedPackage(client, package) )
         {
            // prepare package for usage
            sendStream.setBuffer(package.data, package.datasize);
         }
         else
         {
            delete[] package.data;
            return;
         }
         break;
      case ReliableOrdered:
         // make sure client receives the messages in order (+1 as
         // the new package is at least 1 bigger than the last received one).
         if (false && package.number > (client.lastPackageNumber+1))
         {
            insertOrderedPackage(client, package);

            // check if we have already the package we are waiting for
            PackageList::iterator it = client.orderQueue.begin();
            NetPackage& pack = (*it);
            if (pack.number == client.lastPackageNumber) {
               // extract the actual message
               sendStream.setBuffer(pack.data, pack.datasize);

               // erase the package from the list
               client.orderQueue.erase(it);
            }
            else {
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
      client.lastPackageNumber = package.number;

      NetEvent* event = 0;
      sendStream >> (NetObject**)&event;
      process->onClientEvent(clientid, event, sendStream);
      delete event;

      delete[] package.data;
   }
}

bool NetConnection::doReceive(NetAddress& address, BitStream& recvStream)
{
   int addrLen = SOCKADDR_SIZE;
   char buffer[MAX_BITSTREAM_BUFSIZE];

   int size = recvfrom(sock, buffer, MAX_BITSTREAM_BUFSIZE, 0, (struct sockaddr*)&address.addr, (socklen_t*)&addrLen);
   if ( size == SOCKET_ERROR )
   {
      Console::getInstance().printf("Error during receiving (%d)", getErrorNumber());
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
bool NetConnection::addNewClient(NetAddress& address, bool connecting)
{
   // see if the process wants to accept a connection from this client
   if ( accept )
   {
      if (!connecting)
      {
         // check if the script allows this new player
         Script& script = ScriptManager::getInstance().getTemporaryScript();
         script.prepareCall("Server_onClientConnecting");
         script.run(0,1);

         // the script should return true to allow the new client
         int reason = script.getInteger();
         if ( reason < 0 )
         {
            NetEvent event(deniteEvent);
            BitStream stream;
            stream << &event << reason;
            send(address, &stream, Unreliable);
            return false;
         }
      }

      // build the new address structure
      NetAddress* addr = new NetAddress(address.addr);
      addr->lastTimeRecv = Timer::getInstance().getTick();

      // create new statistics
      addr->pstatistics = new NetStatistics();

      Uint32 i = 0;
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
   for ( Uint32 i = 0 ; i < clients.size(); ++i )
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
   if ( package.number > client.lastPackageNumber || client.lastPackageNumber - package.number > MIN_PACKAGE_NUMBER_DIFFERENCE )
      return true;

   // say max = 250, client.lastPN = 249 and package.nr = 2 -> diff = 247 and thus must be overflow -> set flag
   // then package.nr = 250 -> overflow flag is still set, thus discard the package

   return false;
}

/// \fn NetConnection::convertPackageToBitStream(const NetPackage& package, BitStream& stream)
/// \brief Convert a network package to a bitstream object for tranmission.
void NetConnection::convertPackageToBitStream(const NetPackage& package, BitStream& stream)
{
   stream << package.type << package.isAck << (int)package.number << (int)package.datasize;
   if (package.datasize > 0)
   {
      stream.writeRaw(package.data, package.datasize);
   }
}

/// \fn NetConnection::convertBitStreamToPackage(BitStream& stream, NetPackage& package)
/// \brief Convert a bitstream object to the network package format.
void NetConnection::convertBitStreamToPackage(BitStream& stream, NetPackage& package)
{
   stream >> package.type >> package.isAck >> (int&)package.number >> (int&)package.datasize;
   if (package.datasize > 0)
   {
      package.data = new char[package.datasize];
      stream.readRaw(package.data, package.datasize);
   }
   else
   {
	   package.data = 0;
   }
}

/// \fn NetConnection::sendAck(const NetPackage& package)
/// \brief Send an acknowledgement back to the sender of a reliable package.
void NetConnection::sendAck(NetAddress& client, const NetPackage& package)
{
   BitStream stream;
   NetPackage ackPackage;

   ackPackage.type = Unreliable;
   ackPackage.isAck = true;
   ackPackage.number = package.number;
   ackPackage.datasize = 0;

   convertPackageToBitStream(ackPackage, stream);
   doSend(client, stream);
}

/// \fn NetConnection::removePackageFromResendQueue(NetAddress& client, Uint32 packageNumber)
/// \brief Removes an acknowledged package from the resend queue.
void NetConnection::removePackageFromResendQueue(NetAddress& client, Uint32 packageNumber)
{
   PackageQueue::iterator it = client.resendQueue.begin();
   for ( ; it != client.resendQueue.end(); ++it)
   {
      if ((*it).number == packageNumber)
      {
         delete[] it->data;
         client.resendQueue.erase(it);
         return;
      }
   }

   if (packageNumber > client.lastPackageNumber)
   {
      // this is not possible!
      Console::getInstance().printf("Received an ACK with an invalid package number (max = %d)!", client.lastPackageNumber);
   }
   else
   {
      // if we get here the package wasn't found!
      Console::getInstance().print("Received an ACK for a non-existing package!");
   }

   Console::getInstance().printf("Received package number: %d", packageNumber);
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
      while (it != client.orderQueue.end() && (*it).number < package.number)
      {
         ++it;
      }

      if (it != client.orderQueue.end())
      {
         // note that eventually the package could already been there (if for example
         // the ACK got lost), so make sure the new package number is smaller
         if (package.number < (*it).number)
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
