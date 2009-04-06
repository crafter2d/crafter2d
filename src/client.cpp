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
#include "net/netevent.h"
#include "net/newobjectevent.h"

#include "console.h"
#include "client.h"
#include "player.h"
#include "script.h"
#include "scriptmanager.h"
#include "creature.h"
#include "server.h"
#include "defines.h"

Client::Client(void):
   _pplayer(NULL),
   requests()
{
}

Client::~Client(void)
{
   disconnect();
}

bool Client::connect(const char* server, int port, const char* name)
{
   BitStream stream;

   // setup connection to the server
   conn.create();
   if (!conn.connect(server, port))
      return false;
   conn.setSendAliveMessages(false);
   conn.setAccepting(false);

   _pplayer = new Player();

   // send login command
   NetEvent event(connectEvent);
   stream << &event << name;
   conn.send (&stream);
   return true;
}

void Client::disconnect()
{
   if (conn.isConnected()) {
      BitStream stream;
      DisconnectEvent event;
      stream << &event;
      conn.send(&stream);
   }
}

bool Client::destroy()
{
   Process::destroy();
   conn.setAccepting(true);
   return true;
}

Player& Client::getPlayer()
{
   ASSERT_PTR(_pplayer);
   return *_pplayer;
}

int Client::onClientEvent(int client, NetEvent* event, BitStream& stream)
{
   char buffer[256] = "";
   switch (event->getType())
   {
      case acceptEvent:
         {
            // run the onConnected script
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Client");
            script.prepareCall ("Client_onConnected");
            script.run (0);

            initialized = true;
            break;
         };
      case joinEvent:
         {
            // player joined the game
            Object* controler = NULL;
            JoinEvent* je = dynamic_cast<JoinEvent*>(event);

            // run the onConnected script
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Client");
            script.prepareCall ("Client_onJoined");
            script.addParam(je->getId()+1);
            script.addParam(je->getPlayerName());
            script.run (2);
            break;
         }
      case deniteEvent:
         {
            int reason;
            stream >> reason;

            // run the Client_onConnectionDenite script
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Client");
            script.prepareCall ("Client_onConnectionDenite");
            script.addParam(reason);
            script.run(1);
            break;
         }
      case disconnectEvent:
         {
            DisconnectEvent* je = dynamic_cast<DisconnectEvent*>(event);

            // call the script
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Client");
            script.prepareCall ("Client_onPlayerLeft");
            script.addParam(je->getId()+1);
            script.run (1);
            break;
         }
      case serverdownEvent:
         {
            // server went down, run the onClientConnect script
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Client");
            script.prepareCall ("Client_onServerDown");
            script.run (0);
            break;
         }
      case scriptEvent:
         {
            // run the onClientConnect script
            Script& script = ScriptManager::getInstance().getTemporaryScript();
            script.setSelf (this, "Client");
            script.prepareCall ("Client_onEvent");
            script.addParam(&stream, "BitStream");
            script.run (1);
            break;
         }

      case newobjectEvent:
      {
         NewObjectEvent* pevent = dynamic_cast<NewObjectEvent*>(event);
         handleNewObjectEvent(*pevent);
         break;
      }
      case delobjectEvent: handleDeleteObjectEvent(stream);    break;
      case updobjectEvent: handleUpdateObjectEvent(stream);    break;
      case reqobjectEvent: handleRequestObjectEvent(stream);   break;
   }
   
   return 0;
}

void Client::handleNewObjectEvent(NewObjectEvent& event)
{
   // a new object has been made on the server and 
   // is now also known on the client
   SceneObject* obj = event.getObject();
   obj->setReplica();
   obj->create();

   if ( graph.find(obj->getName()) == 0 )
   {
      if ( obj->getRuntimeInfo()->getName() == "World" )
      {
         _pplayer->initialize(*(World*)obj);

         getSceneGraph().setWorld((World*)obj);
   
         // run the onWorldChanged script
         Script& script = ScriptManager::getInstance().getTemporaryScript();
         script.setSelf(this, "Client");
         script.prepareCall("Client_onWorldChanged");
         script.run(0);
      }
      else
      {
         SceneObject* parent = graph.find(event.getParent().c_str());
         if ( parent != NULL )
            parent->add(obj);
      }
   }
}

void Client::handleDeleteObjectEvent(BitStream& stream)
{
   char object[256] = {0};
   stream >> object;
   SceneObject* node = graph.find(object);
   if ( node != NULL )
   {
      node->destroy();
      delete node;
   }
}

void Client::handleUpdateObjectEvent(BitStream& stream)
{
   char object[256] = {0};
   stream >> object;
   SceneObject* pobject = graph.find(object);
   if ( pobject == NULL )
   {
      if ( requests.find(object) == requests.end() )
      {
         // unknown object, must have been generated before the player entered the game
         NetEvent event(reqobjectEvent);
         BitStream stream;
         stream << &event << object;
         getConnection()->send(&stream);

         requests[object] = true;
      }
   }
   else
      pobject->unpack(stream);
}

void Client::handleRequestObjectEvent(BitStream& stream)
{
   char parent[256] = {0};
   stream >> parent;

   Object* obj;
   stream >> (NetObject**)&obj;
   obj->setReplica();
   obj->create();

   SceneObject* pparent = graph.find(parent);
   pparent->add(obj);

   requests.erase(parent);
}
