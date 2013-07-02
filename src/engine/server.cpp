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
#include "server.h"

#include "core/smartptr/autoptr.h"
#include "core/log/log.h"
#include "core/smartptr/scopedvalue.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"
#include "engine/net/events/aggregateevent.h"
#include "engine/net/events/connectevent.h"
#include "engine/net/events/connectreplyevent.h"
#include "engine/net/events/disconnectevent.h"
#include "engine/net/events/joinevent.h"
#include "engine/net/events/scriptevent.h"
#include "engine/net/events/newobjectevent.h"
#include "engine/net/events/requestobjectevent.h"
#include "engine/net/events/viewportevent.h"
#include "engine/net/events/serverdownevent.h"
#include "engine/net/events/actionevent.h"
#include "engine/net/events/worldchangedevent.h"
#include "engine/net/netstream.h"
#include "engine/net/netobjectstream.h"
#include "engine/physics/simulationfiller.h"
#include "engine/physics/simulator.h"
#include "engine/world/world.h"

#include "controller.h"
#include "player.h"
#include "serverdirtyset.h"
#include "servercontentmanager.h"

Server::Server():
   Process(),
   clients(),
   mWorldObserver(*this),
   mActiveClient(-1)
{
   setContentManager(new ServerContentManager(*this));
}

Server::~Server()
{
}

bool Server::destroy()
{
   if (conn.isConnected())
   {
      conn.setAccepting(false);

      ServerDownEvent event;
      sendToAllClients(event);

      mpScript->run(UTEXT("onShutdown"));
   }
   
   return Process::destroy();
}

bool Server::listen(int port)
{
   if ( !conn.isConnected() )
   {
      if ( !conn.listen(port) )
      {
         return false;
      }
      conn.setAccepting(true);
   }
   return true;
}

/// \fn Server::update (Uint32 tick)
/// \brief Main update routine of the server process.
///
/// Updates the server side scenegraph and sends the changes to the connected clients.
/// At the end it handles the incomming events.
void Server::update(float delta)
{
   conn.update();

   // update the graph
   if ( hasWorld() )
   {
      ServerDirtySet dirtyset;
      getWorld().update(dirtyset, delta);

      // send changes to the clients
      ClientMap::iterator it = clients.begin();
      for ( ; it != clients.end(); ++it )
      {
         AggregateEvent event;
         dirtyset.collect(event);

         if ( !event.isEmpty() )
         {
            int clientid = it->first;
            conn.send(clientid, event);
         }
      }
   }
}

// ----------------------------------
// - Notifications
// ----------------------------------

void Server::notifyWorldChanged()
{
   getWorld().attach(mWorldObserver);

   WorldChangedEvent event(getWorld());
   sendToAllClients(event);
}

// ----------------------------------
// - Sending
// ----------------------------------

void Server::sendToAllClients(const NetObject& object)
{
   // copying a buffer is faster than packaging per client (trade-off)
   BufferedStream bufstream;
   NetObjectStream stream(bufstream);
   stream << object;

   sendToAllClients(stream);
}

void Server::sendToAllClients(const NetStream& stream)
{
   // send changes to the clients
   ClientMap::iterator it = clients.begin();
   for ( ; it != clients.end(); ++it)
   {
      int clientid = it->first;
      conn.send(clientid, stream);
   }
}

void Server::sendToActiveClient(const NetObject& object)
{
  conn.send(mActiveClient, object);
}

void Server::sendScriptEventToAllClients(const NetStream& stream)
{
   ScriptEvent event(stream);
   sendToAllClients(event);
}

// ----------------------------------
// -- Event handling
// ----------------------------------

/// \fn Server::onNetEvent(int client, const NetEvent& event)
/// \brief Handles the incomming events.
void Server::onNetEvent(int client, const NetEvent& event)
{
   ScopedValue<int> value(&mActiveClient, client, -1);

   switch ( event.getType() )
   {
      case connectEvent:
         {
            const ConnectEvent& connectevent = dynamic_cast<const ConnectEvent&>(event);
            handleConnectEvent(connectevent);
            break;
         }
      case disconnectEvent:
         {
            AutoPtr<Player> player = clients[client];

            // run the onClientConnect script
            Variant args[2];
            args[0].setInt(client);
            args[1].setObject(mpScript->resolve(player.getPointer()));
            mpScript->run(UTEXT("onClientDisconnect"), 2, args);

            // remove the player from the client list
            ClientMap::iterator it = clients.find(client);
            clients.erase(it);

            conn.disconnect(client);

            // fill in the event and put it in the stream
            DisconnectEvent event(client);
            sendToAllClients(event);
            break;
         }
      case scriptEvent:
         {
            const ScriptEvent& scriptevent = dynamic_cast<const ScriptEvent&>(event);

            DataStream& datastream = const_cast<DataStream&>(scriptevent.getStream());
            NetStream stream(datastream);

            // run the onClientConnect script
            Player* player = clients[client];
            Variant args[2];
            args[0].setObject(mpScript->resolve(player));
            args[1].setObject(mpScript->instantiate(UTEXT("engine.net.NetStream"), &stream));
            mpScript->run(UTEXT("onEvent"), 2, args);
            break;
         }
      case actionEvent:
         {
            // find the player object
            Player* pplayer = clients[client];
            ASSERT_PTR(pplayer);

            const ActionEvent& inputevent = dynamic_cast<const ActionEvent&>(event);
            Controller& controller = pplayer->getController().getController();

            controller.requestAction(inputevent);
            break;
         }
      case reqobjectEvent:
         {
            const RequestObjectEvent& request = dynamic_cast<const RequestObjectEvent&>(event);

            Entity* pentity = getWorld().findEntity(request.getId());
            if ( pentity == NULL )
            {
               Log::getInstance().error("Server - Could not find matching object for request");
            }
            else
            {
               NewObjectEvent event(*pentity);
               sendToActiveClient(event);
            }
            break;
         }
      case viewportEvent:
         {
            const ViewportEvent& viewportevent = dynamic_cast<const ViewportEvent&>(event);
            handleViewportEvent(viewportevent);

            break;
         }
      default:
         Log::getInstance().error("Server: Received an unknown message.");
         break;
   }
}

/// \fn Server::addPlayer(int client, Player* pplayer)
/// \brief Adds a new player to the client list
void Server::addPlayer(int clientid, Player* pplayer)
{
   clients[clientid] = pplayer;
   pplayer->setClientId(clientid);

   // send the reply to the connecting client
   ConnectReplyEvent event(ConnectReplyEvent::eAccepted);
   conn.send(clientid, event);

   // notify other players about the new player
   JoinEvent join(clientid);
   BufferedStream bufstream;
   NetObjectStream stream(bufstream);
   stream << join;

   ClientMap::iterator it = clients.begin();
   for ( ; it != clients.end(); ++it)
   {
      Player* pother = it->second;
      if ( pother!= pplayer )
      {
         conn.send(pother->getClientId(), stream);
      }
   }
}

void Server::handleConnectEvent(const ConnectEvent& event)
{
   // check if the script allows this new player
   Variant retval = mpScript->run(UTEXT("onClientConnecting"));
   int reason = retval.asInt();
   if ( reason != 0 )
   {
      ConnectReplyEvent event(ConnectReplyEvent::eDenite, reason);
      conn.send(mActiveClient, event);
   }
   else
   {
      // create the player object
      Player* pplayer = new Player();
      addPlayer(mActiveClient, pplayer);

      // run the onClientConnect script
      Variant arg(mpScript->instantiate(UTEXT("engine.game.Player"), pplayer));
      mpScript->run(UTEXT("onClientConnect"), 1, &arg);
   }
}

void Server::handleViewportEvent(const ViewportEvent& event)
{
   Player& player = *clients[mActiveClient];
   //event.update(player.getViewport());
}
