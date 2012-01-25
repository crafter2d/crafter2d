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

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "net/events/connectevent.h"
#include "net/events/connectreplyevent.h"
#include "net/events/disconnectevent.h"
#include "net/events/joinevent.h"
#include "net/events/scriptevent.h"
#include "net/events/newobjectevent.h"
#include "net/events/requestobjectevent.h"
#include "net/events/viewportevent.h"
#include "net/events/serverdownevent.h"
#include "net/events/actionevent.h"
#include "net/events/worldchangedevent.h"

#include "physics/simulationfiller.h"
#include "physics/simulator.h"

#include "world/world.h"

#include "controller.h"
#include "player.h"
#include "sceneobjectdirtyset.h"
#include "scopedvalue.h"

Server::Server():
   Process(),
   clients(),
   mWorldObserver(*this),
   mActiveClient(-1)
{
}

Server::~Server()
{
}

bool Server::create(const std::string& name)
{
   return Process::create(name);
}

bool Server::destroy()
{
   if (conn.isConnected())
   {
      ServerDownEvent event;
      sendToAllClients(event);
   }
   
   return Process::destroy();
}

void Server::listen(int port)
{
   if ( !conn.isConnected() )
   {
      conn.create(port);
      conn.setAccepting(true);
   }
}

void Server::shutdown()
{
   if ( conn.isConnected() )
   {
      // prevent clients from connecting
      conn.setAccepting(false);
      conn.disconnect();

      // call the shutdown function
      mpScript->run("onShutdown");
   }
}

/// \fn Server::update (Uint32 tick)
/// \brief Main update routine of the server process.
///
/// Updates the server side scenegraph and sends the changes to the connected clients.
/// At the end it handles the incomming events.
void Server::update(float delta)
{
   // update the connection
   if (!conn.isConnected())
      return;
   conn.update();

   // update the graph
   if ( hasWorld() )
   {
      SceneObjectDirtySet dirtyset;
      getWorld().update(dirtyset, delta);

      // send changes to the clients
      ClientMap::iterator it = clients.begin();
      for ( ; it != clients.end(); ++it )
      {
         conn.setClientId(it->first);
         dirtyset.send(conn);
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

void Server::sendToAllClients(NetObject& object)
{
   BitStream stream;
   stream << &object;
   sendToAllClients(stream);
}

void Server::sendToAllClients(BitStream& stream)
{
   // send changes to the clients
   ClientMap::iterator it = clients.begin();
   for ( ; it != clients.end(); ++it)
   {
      conn.setClientId(it->first);
      conn.send(&stream);
   }
}

void Server::sendToActiveClient(NetObject& object)
{
  conn.setClientId(mActiveClient);
  conn.send(&object);
}

void Server::sendScriptEventToAllClients(BitStream* pstream)
{
   ScriptEvent event(pstream);

   BitStream stream;
   stream << &event;

   sendToAllClients(stream);
}

// ----------------------------------
// -- Event handling
// ----------------------------------

int Server::allowNewConnection()
{
   // check if the script allows this new player
   mpScript->run("onClientConnecting");

   // the script should return true to allow the new client
   int reason = mpScript->getInteger();
   return reason;
}

/// \fn Server::onClientEvent(int client, const NetEvent& event)
/// \brief Handles the incomming events.
int Server::onClientEvent(int client, const NetEvent& event)
{
   ScopedValue<int> value(mActiveClient, client, -1);

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
            mpScript->addParam((int)client);
            mpScript->addParam("Player", player.getPointer());
            mpScript->run("onClientDisconnect");

            // remove the player from the client list
            ClientMap::iterator it = clients.find(client);
            clients.erase(it);

            // fill in the event and put it in the stream
            DisconnectEvent event(client);
            sendToAllClients(event);
            break;
         }
      case scriptEvent:
         {
            const ScriptEvent& scriptevent = dynamic_cast<const ScriptEvent&>(event);

            AutoPtr<BitStream> stream = scriptevent.getStream();

            // run the onClientConnect script
            Player* player = clients[client];
            mpScript->addParam("Player", player);
            mpScript->addParam("BitStream", &stream);
            mpScript->run("onEvent");
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

   return 0;
}

/// \fn Server::addPlayer(int client, Player* pplayer)
/// \brief Adds a new player to the client list
void Server::addPlayer(int clientid, Player* pplayer)
{
   clients[clientid] = pplayer;
   pplayer->setClientId(clientid);

   // send the reply to the connecting client
   ConnectReplyEvent event(ConnectReplyEvent::eAccepted);
   conn.setClientId(clientid);
   conn.send(&event);

   // notify other players about the new player
   JoinEvent join(clientid, pplayer->getName());

   BitStream stream;
   stream << &join;
   ClientMap::iterator it = clients.begin();
   for ( ; it != clients.end(); ++it)
   {
      Player* pother = it->second;
      if (pother!= pplayer)
      {
         conn.setClientId(pother->getClientId());
         conn.send(&stream);
      }
   }
}

void Server::handleConnectEvent(const ConnectEvent& event)
{
   // create the player object
   Player* pplayer = new Player();
   pplayer->setName(event.getName());
   addPlayer(mActiveClient, pplayer);

   // run the onClientConnect script
   mpScript->addParam("Player", pplayer);
   mpScript->run("onClientConnect");
}

void Server::handleViewportEvent(const ViewportEvent& event)
{
   Player& player = *clients[mActiveClient];
   event.update(player.getViewport());
}
