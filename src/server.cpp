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

#include "actionmap.h"
#include "console.h"
#include "player.h"
#include "script.h"
#include "scriptmanager.h"

IMPLEMENT_REPLICATABLE(JoinEventId, JoinEvent, NetEvent)

JoinEvent::JoinEvent():
   NetEvent(joinEvent),
   playerName(),
   id(-1)
{
}

JoinEvent::JoinEvent(int i, const std::string& name):
   NetEvent(joinEvent),
   playerName(name),
   id(i)
{
}

void JoinEvent::pack(BitStream& stream) const
{
   NetEvent::pack(stream);
   stream << id << playerName.c_str();
}

void JoinEvent::unpack(BitStream& stream)
{
   char name[256];

   NetEvent::unpack(stream);
   stream >> id >> name;

   playerName = name;
}

/******************************************************
 * Disconnect class
 */

IMPLEMENT_REPLICATABLE(DisconnectEventId, DisconnectEvent, NetEvent)

DisconnectEvent::DisconnectEvent(): 
   NetEvent(disconnectEvent),
   _id(-1)
{
}

DisconnectEvent::DisconnectEvent(int id):
   NetEvent(disconnectEvent),
   _id(id)
{
}

void DisconnectEvent::pack(BitStream& stream) const
{
   NetEvent::pack(stream);
   stream << _id;
}

void DisconnectEvent::unpack(BitStream& stream)
{
   NetEvent::unpack(stream);
   stream >> _id;
}

/******************************************************
 * Server class
 */

Server::Server(void)
{
}

Server::~Server(void)
{
}

bool Server::create()
{
   Process::create();
   graph.setNotify();
   return true;
}

bool Server::destroy()
{
   if (conn.isConnected())
   {
      NetEvent event(serverdownEvent);
      BitStream stream;

      // let all connected people know that the server is going down
      stream << &event;
      sendToAllClients(stream);
   }

   Process::destroy();
   return true;
}

void Server::shutdown()
{
   if ( conn.isConnected() )
   {
      // prevent clients from connecting
      conn.setAccepting(false);

      // call the shutdown function
      Script& script = ScriptManager::getInstance().getTemporaryScript();
      script.setSelf (this, "Server");
      script.prepareCall ("Server_onShutdown");
      script.run();
   }
}

/// \fn Server::update (Uint32 tick)
/// \brief Main update routine of the server process.
///
/// Updates the server side scenegraph and sends the changes to the connected clients.
/// At the end it handles the incomming events.
void Server::update (Uint32 tick)
{
   // update the connection
   if (!conn.isConnected())
      return;
   conn.update(tick);

   // update the graph
   graph.setConnection(&conn);
   graph.update(tick);

   // send changes to the clients
   ClientMap::iterator it = clients.begin();
   for ( ; it != clients.end(); ++it)
   {
      conn.setClientId(it->first);
      graph.transmitChanges();
   }
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

void Server::sendScriptEventToAllClients(BitStream* stream)
{
   BitStream s;
   NetEvent event(scriptEvent);
   s << &event << stream;
   sendToAllClients(s);
}

/// \fn Server::onClientEvent(int client, NetEvent* event, BitStream& stream)
/// \brief Handles the incomming events.
int Server::onClientEvent(int client, NetEvent* event, BitStream& stream)
{
   switch (event->getType()) {
      case connectEvent:
         {
            char name[256];
            stream >> name;

            // create the player object
            Player* player = new Player();
            player->name = name;
            addPlayer(client, player);

            // run the onClientConnect script
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Server");
            script.prepareCall ("Server_onClientConnect");
            script.addParam(player, "Player");
            script.run (1);
            break;
         }
      case disconnectEvent:
         {
            stream.clear();
            Player* player = clients[client];

            // run the onClientConnect script
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Server");
            script.prepareCall ("Server_onClientDisconnect");
            script.addParam ((int)client);
            script.addParam (player, "Player");
            script.run (2);

            // remove the player from the client list
            ClientMap::iterator it = clients.find(client);
            clients.erase(it);

            // fill in the event and put it in the stream
            DisconnectEvent event(client);
            stream << &event;

            // send the disconnect message to the other clients
            it = clients.begin();
            for ( ; it != clients.end(); ++it)
            {
               Player* otherPlayer = it->second;
               conn.setClientId(otherPlayer->client);
               conn.send(&stream);
            }

            // delete the player object
            delete player;
            break;
         }
      case scriptEvent:
         {
            // run the onClientConnect script
            Player* player = clients[client];
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Server");
            script.prepareCall ("Server_onEvent");
            script.addParam(player, "Player");
            script.addParam(&stream, "BitStream");
            script.run (2);
            break;
         }
      case inputEvent: 
         {
            // find the player object
            Player* player = clients[client];
            if (player == NULL)
               Console::getInstance().print("Invalid player input event.");
            else {
               InputEvent* ie = (InputEvent*)event;
               actionMap->process((InputEvent&)*ie, (Object*)player->controler);
            }
            break;
         }
      case reqobjectEvent:
         {
            char objname[256];
            stream >> objname;
            SceneObject* obj = graph.find(objname);

            // fill in & send the reponse of the requestEvent
            NetEvent event(reqobjectEvent);
            stream.clear();
            stream << &event;
            stream << obj->getParent()->getName();
            stream << (NetObject*)obj;

            Player* player = clients[client];
            conn.setClientId(player->client);
            conn.send(&stream);
            break;
         }
      case scrollEvent:
         {
            int x, y;
            stream >> x >> y;

            Player* player = clients[client];
            player->getViewport().setPosition(x, y);
            break;
         }
      default:
         Console::getInstance().print("Server: Received an unknown message.");
         break;
   }
   return 0;
}

/// \fn Server::addPlayer(NetAddress* client, Player* player)
/// \brief Adds a new player to the client list
void Server::addPlayer(int client, Player* player)
{
   clients[client] = player;
   player->client = client;
   if ( player->name.empty() )
   {
      char name[32];

      sprintf (name, "player%d", clients.size());
      player->name = name;
   }

   BitStream stream;

   // send the reply to the connecting client
   NetEvent event(acceptEvent);
   stream << &event;
   conn.setClientId(client);
   conn.send(&stream);
   stream.clear();

   // notify other players about the new player
   JoinEvent join(player->client, player->name);
   stream << (NetObject*)&join;
   ClientMap::iterator it = clients.begin();
   for ( ; it != clients.end(); ++it)
   {
      Player* otherPlayer = it->second;
      if (otherPlayer != player)
      {
         conn.setClientId(otherPlayer->client);
         conn.send(&stream);
      }
   }
}
