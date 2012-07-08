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
#include "core/streams/arraystream.h"
#include "core/streams/bufferedstream.h"
#include "core/containers/listiterator.h"
#include "core/log/log.h"
#include "core/system/timer.h"

#include "events/netevent.h"
#include "netaddress.h"
#include "netobserver.h"
#include "netstatistics.h"
#include "netobjectstream.h"
#include "netstream.h"
#include "netsocketexception.h"

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

NetPackage* construct()
{
   return new NetPackage();
}

/******************************************************
 * NetConnection class
 */

NetConnection::NetConnection(NetObserver& observer):
   mObserver(observer),
   mClients(),
   mAllocator(),
   mSocket(),
   mFlags(0)
{
   mAllocator.setConstructFunc(construct);
}

NetConnection::~NetConnection()
{
}

/// \fn NetConnection::listen(int port)
/// \brief Sets up a connection to listen to incomming messages on port 'port'
/// \param port port number to bind to
///
/// This method creates a socket and optionaly binds it to a port.
bool NetConnection::listen(int port)
{
   if ( mSocket.bind(port) )
   {
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
   int clientid = -1;

   NetAddress address;
   if ( mSocket.resolve(address, serverName)
     && mSocket.connect(address, port) )
   {
      NetAddress& client = addNewClient(address);
      SET_FLAG(mFlags, eConnected);
      clientid = client.index;
   }
   
   return clientid;
}

/// \fn NetConnection::disconnect(int client)
/// \brief Closes the connection to the given client.
void NetConnection::disconnect(int client)
{
   mClients.remove(client);
}

void NetConnection::shutdown()
{
   if ( IS_SET(mFlags, eConnected) )
   {
      CLEAR_FLAG(mFlags, eConnected);

      mSocket.close();

      // remove the clients
      mClients.clear();
   }
}

/// \fn NetConnection::update()
/// \brief Updates the message queues etc.
void NetConnection::update()
{
   ASSERT(IS_SET(mFlags, eConnected));

   Timer& timer = TIMER;
   
   // receive pending messages
   receive();
   
   // resend messages if necessary
   mClients.rewind();
   while ( mClients.hasNext() )
   {
      NetAddress& client = mClients.getNext();

#ifdef JENGINE_CONNTIMEOUT
      if ((timer.getTick() - client.lastTimeRecv) > MAX_TIME_BETWEEN_RECV)
      {
         // the client has been time out, remove the bastard
         Log::getInstance().error("Player %d has been timed out!", index);
      }
      else
#endif
      {
         process(client);

         sendAck(client);
         sendAlive(client, timer.getTick());

         if ( client.pstatistics )
         {
            client.pstatistics->update(timer.getTick());
         }
      }
   }
}

/// \fn NetConnection::process(int clientid)
/// \brief Process all pending messages.
void NetConnection::process(NetAddress& client)
{
   if ( client.waitAttempt > 0 )
   {
      client.waitTimer -= TIMER.getTick();
      if ( client.waitTimer > 0.0f )
      {
         return;
      }
      else if ( client.waitTimer <= 0.0f )
      {
         sendRequest(client);

         client.waitAttempt++;
         client.waitTimer = client.waitAttempt * client.waitAttempt;
      }
   }

   ListIterator<PackageHandle> it = client.orderQueue.getFront();
   while ( it.isValid() )
   {
      PackageHandle& handle = *it;

      if ( handle->getNumber() == client.nextPackageNumber )
      {
         client.waitAttempt = 0;

         // first remove it from the pile
         PackageHandle package(handle);
         it.remove();

         processPackage(client, *package);
      }
      else if ( handle->getNumber() > client.nextPackageNumber )
      {
         // we miss a package, start the wait timer
         client.waitAttempt++;
         client.waitTimer = client.waitAttempt * client.waitAttempt;
         break;
      }
   }
}

void NetConnection::processPackage(NetAddress& client, NetPackage& package)
{
   NetObject* pobject = NULL;
   ArrayStream arraystream(package.getData(), package.getDataSize());
   NetObjectStream stream(arraystream);
   stream >> &pobject;

   AutoPtr<NetEvent> event(static_cast<NetEvent*>(pobject));
   mObserver.onEvent(client.index, *event);

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

   try
   {
      send(client, *package);

      // when reliability is requested, save the package in the resend queue
      if ( reliability >= NetPackage::eReliableSequenced )
      {
         client.resendQueue.push_back(package);
      }
   }
   catch ( NetSocketException* pexception )
   {
      switch ( pexception->getError() )
      {
         case NetSocketException::eConnReset:
            {
               mClients.remove(client.index);
            }
            break;
      }
   }
}

/// \fn NetConnection::sendAck(const NetPackage& package)
/// \brief Send an acknowledgement back to the sender of a reliable package.
void NetConnection::sendAck(NetAddress& client)
{
   NetPackage ackPackage(NetPackage::eAck, NetPackage::eUnreliable, client.nextPackageNumber-1);

   send(client, ackPackage);
}

/// \fn NetConnection::sendRequest(NetAddress& client)
/// \brief Send a request for the next package of the given client
void NetConnection::sendRequest(NetAddress& client)
{
   NetPackage ackPackage(NetPackage::eRequest, NetPackage::eUnreliable, client.nextPackageNumber);

   send(client, ackPackage);
}

/// \fn NetConnection::sendAlive(NetAddress& client, float tick)
/// \brief Sends an alive system message, when no message has been send for some time.
void NetConnection::sendAlive(NetAddress& client, float tick)
{
   if ( IS_SET(mFlags, eKeepAlive) && (tick - client.lastTimeSend >= ALIVE_MSG_INTERVAL) )
   {
      NetPackage package(NetPackage::eAlive, NetPackage::eUnreliable, 0);
      send(client, package);
   }
}

void NetConnection::send(NetAddress& client, const NetPackage& package)
{
   int bytes = mSocket.send(client, package);

   client.lastTimeSend = TIMER.getTick();
   if ( client.pstatistics )
   {
      client.pstatistics->send(bytes);
   }
}

/// \fn NetConnection::receive()
/// \brief Receive data from the socket
void NetConnection::receive()
{
   NetAddress address;
   while ( mSocket.select(true, false) )
   {
      int bytes = -1;
      PackageHandle package(mAllocator);

      try
      {
         bytes = mSocket.receive(address, *package);
         ASSERT(package.hasObject());

         NetAddress& client = findOrCreate(address); 
         client.lastTimeRecv = Timer::getInstance().getTick();

         if ( client.pstatistics )
         {
            client.pstatistics->received(bytes);
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
            case NetPackage::eRequest:
               {
                  // add the requested message at the front of the queue,
                  // so it gets checked first
                  client.orderQueue.addFront(package);
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
      catch ( NetSocketException* pexception )
      {
         switch ( pexception->getError() )
         {
            case NetSocketException::eConnReset:
               {
                  NetAddress *pclient = mClients.find(address); 
                  if ( pclient != NULL )
                  {
                     mClients.remove(pclient->index);
                  }
                  // else nothing to close
                  break;
               }
         }
      }
   }
}

/// \fn NetConnection::addNewClient(const NetAddress& address)
/// \brief Add new client to the client list.
/// \param address Address of the new client thats connected
/// \returns the address of the new connection
NetAddress& NetConnection::addNewClient(const NetAddress& address)
{
   NetAddress* paddr = new NetAddress(address.addr);
   paddr->lastTimeRecv = Timer::getInstance().getTick();
   // paddr->pstatistics = new NetStatistics();
   
   return mClients.add(paddr);
}

/// \fn NetConnection::findOrCreate(const NetAddress& client)
/// \brief Finds or creates an address based on the given client info
/// \return the clientid of the client or INVALID_CLIENTID when not found (e.g. when not accepting)
NetAddress& NetConnection::findOrCreate(const NetAddress& client)
{
   NetAddress* pclient = mClients.find(client);
   if ( pclient == NULL )
   {
      if ( IS_SET(mFlags, eAccept) )
      {
         pclient = &addNewClient(client);
      }
   }
   
   ASSERT_PTR(pclient);
   return *pclient;
}
