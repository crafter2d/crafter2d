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
#include "net/events/connectevent.h"
#include "net/events/connectreplyevent.h"
#include "net/events/deleteobjectevent.h"
#include "net/events/updateobjectevent.h"
#include "net/events/requestobjectevent.h"
#include "net/events/scriptevent.h"

#include "world/world.h"

#include "autoptr.h"
#include "console.h"
#include "client.h"
#include "player.h"
#include "script.h"
#include "scriptmanager.h"
#include "creature.h"
#include "server.h"
#include "defines.h"

Client::Client(void):
   Process(),
   mpPlayer(NULL),
   requests()
{
}

Client::~Client(void)
{
   disconnect();
}

bool Client::connect(const char* server, int port, const char* name)
{
   // setup connection to the server
   conn.create();
   if (!conn.connect(server, port))
      return false;
   conn.setSendAliveMessages(false);
   conn.setAccepting(false);

   mpPlayer = new Player();

   // send login command
   ConnectEvent event(name);
   conn.send (&event);
   return true;
}

void Client::disconnect()
{
   if ( conn.isConnected() )
   {
      DisconnectEvent event;
      conn.send(&event);
   }
}

bool Client::destroy()
{
   Process::destroy();
   conn.setAccepting(true);
   return true;
}

void Client::update(float delta)
{
   Process::update(delta);

   graph.updateClient(delta);
}

Player& Client::getPlayer()
{
   ASSERT_PTR(mpPlayer);
   return *mpPlayer;
}

int Client::onClientEvent(int client, const NetEvent& event)
{
   switch ( event.getType() )
   {
      case connectReplyEvent:
         {
            const ConnectReplyEvent& crevent = dynamic_cast<const ConnectReplyEvent&>(event);
            handleConnectReplyEvent(crevent);           
            break;
         };
      case joinEvent:
         {
            const JoinEvent& joinevent = dynamic_cast<const JoinEvent&>(event);
            handleJoinEvent(joinevent);
            break;
         }
      case disconnectEvent:
         {
            const DisconnectEvent& disconnectevent = dynamic_cast<const DisconnectEvent&>(event);
            handleDisconnectEvent(disconnectevent);
            break;
         }
      case serverdownEvent:
         {
            handleServerdownEvent();
            break;
         }
      case scriptEvent:
         {
            const ScriptEvent& scriptevent = dynamic_cast<const ScriptEvent&>(event);
            handleScriptEvent(scriptevent);
            break;
         }
      case newobjectEvent:
         {
            const NewObjectEvent& newobjectevent = dynamic_cast<const NewObjectEvent&>(event);
            handleNewObjectEvent(newobjectevent);
            break;
         }
      case delobjectEvent: 
         {
            const DeleteObjectEvent& delobjectevent = dynamic_cast<const DeleteObjectEvent&>(event);
            handleDeleteObjectEvent(delobjectevent);
            break;
         }
      case updobjectEvent: 
         {
            const UpdateObjectEvent& updateobjectevent = dynamic_cast<const UpdateObjectEvent&>(event);
            handleUpdateObjectEvent(updateobjectevent);
            break;
         }
   }
   
   return 0;
}

void Client::handleConnectReplyEvent(const ConnectReplyEvent& event)
{
   Script& script = ScriptManager::getInstance().getTemporaryScript();
   script.setSelf (this, "Client");

   switch ( event.getReply() )
   {
      case ConnectReplyEvent::eAccepted:
         {
            // run the onConnected script
            script.prepareCall ("Client_onConnected");
            script.run (0);

            initialized = true;
            break;
         }
      case ConnectReplyEvent::eDenite:
         {
             // run the Client_onConnectionDenite script
             script.prepareCall ("Client_onConnectionDenite");
             script.addParam(event.getReason());
             script.run(1);
             break;
         }
   }
}

void Client::handleDisconnectEvent(const DisconnectEvent& event)
{
   // call the script
   Script& script = ScriptManager::getInstance().getTemporaryScript();
   script.setSelf (this, "Client");
   script.prepareCall("Client_onPlayerLeft");
   script.addParam(event.getId()+1);
   script.run (1);
}

void Client::handleJoinEvent(const JoinEvent& event)
{
   // run the onConnected script
   Script& script = ScriptManager::getInstance().getTemporaryScript();
   script.setSelf (this, "Client");
   script.prepareCall("Client_onJoined");
   script.addParam(event.getId()+1);
   script.addParam(event.getPlayerName());
   script.run(2);
}

void Client::handleServerdownEvent()
{
   // server went down, run the onClientConnect script
   Script& script = ScriptManager::getInstance().getTemporaryScript();
   script.setSelf (this, "Client");
   script.prepareCall ("Client_onServerDown");
   script.run (0);
}

void Client::handleNewObjectEvent(const NewObjectEvent& event)
{
   // a new object has been made on the server and 
   // is now also known on the client
   AutoPtr<SceneObject> obj = event.getObject();
   obj->setReplica();
   obj->create();

   if ( graph.find(obj->getName().c_str()) == 0 )
   {
      if ( World::isWorld(*obj) )
      {
         mpPlayer->initialize((World&)*obj);

         getSceneGraph().setWorld((World*)obj.release());
   
         // run the onWorldChanged script
         Script& script = ScriptManager::getInstance().getTemporaryScript();
         script.setSelf(this, "Client");
         script.prepareCall("Client_onWorldChanged");
         script.run(0);
      }
      else
      {
         SceneObject* pparent = graph.find(event.getParent().c_str());
         if ( pparent != NULL )
         {
            pparent->add(obj.release());
         }
         else
         {
            UNREACHABLE("Parent of object not found.")
         }
      }
   }
}

void Client::handleDeleteObjectEvent(const DeleteObjectEvent& event)
{
   SceneObject* node = graph.find(event.getName().c_str());
   if ( node != NULL )
   {
      node->destroy();
      delete node;
   }
}

void Client::handleUpdateObjectEvent(const UpdateObjectEvent& event)
{
   const std::string& objectname = event.getName();
   SceneObject* pobject = graph.find(objectname.c_str());
   if ( pobject == NULL )
   {
      if ( requests.find(objectname.c_str()) == requests.end() )
      {
         // unknown object, must have been generated before the player entered the game
         RequestObjectEvent event(objectname);
         getConnection()->send(&event);

         requests[objectname.c_str()] = true;
      }
   }
   else
   {
      event.update(*pobject);
   }
}

void Client::handleScriptEvent(const ScriptEvent& event)
{
   AutoPtr<BitStream> stream(event.getStream());

   // run the onClientConnect script
   Script& script = ScriptManager::getInstance().getTemporaryScript();
   script.setSelf (this, "Client");
   script.prepareCall ("Client_onEvent");
   script.addParam(stream.getPointer(), "BitStream");
   script.run (1);
}