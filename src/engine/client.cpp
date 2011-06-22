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
#include "client.h"
#ifndef JENGINE_INLINE
#  include "client.inl"
#endif

#include <GL/GLU.h>

#include "core/smartptr/autoptr.h"
#include "core/log/log.h"
#include "core/math/color.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "net/netevent.h"
#include "net/newobjectevent.h"
#include "net/events/connectevent.h"
#include "net/events/connectreplyevent.h"
#include "net/events/deleteobjectevent.h"
#include "net/events/disconnectevent.h"
#include "net/events/joinevent.h"
#include "net/events/updateobjectevent.h"
#include "net/events/requestobjectevent.h"
#include "net/events/namechangeobjectevent.h"
#include "net/events/scriptevent.h"

#include "world/world.h"
#include "world/worldrenderer.h"

#include "player.h"
#include "creature.h"
#include "core/defines.h"
#include "sceneobject.h"
#include "actionmap.h"
#include "keymap.h"
#include "opengl.h"

Client::Client():
   Process(),
   mpWindow(NULL),
   mWindowListener(*this),
   mSoundManager(),
   mpWorldRenderer(NULL),
   mpPlayer(NULL),
   mpKeyMap(NULL),
   mpInput(NULL),
   mRequests()
{
}

Client::~Client()
{
   disconnect();
}

// - Creation

bool Client::create(const std::string& name)
{
   bool success = Process::create(name);
   if ( success )
   {
      ASSERT_PTR(mpWindow);

      mpWindow->addListener(mWindowListener);
      if ( !mpWindow->create("GameWindow", 800, 600, 32, false) )
      {
         return false;
      }

      if ( !initOpenGL() )
      {
         return false;
      }

      Log& log = Log::getInstance();
      log << "\n-- Initializing Sound --\n\n";

      // initialize the sound system
      mSoundManager.initialize();
   }

   return success;
}

bool Client::destroy()
{
   conn.setAccepting(false);

   mSoundManager.destroy();

   delete mpWindow;
   mpWindow = NULL;

   return Process::destroy();
}

bool Client::connect(const char* server, int port, const char* name)
{
   // setup connection to the server
   conn.create();
   if (!conn.connect(server, port))
      return false;

   conn.setSendAliveMessages(false);
   conn.setAccepting(false);

   graph.getRoot().setReplica();

   mpPlayer = new Player();
   if ( name != NULL )
   {
      mpPlayer->setName(name);
   }

   // send login command
   ConnectEvent event(mpPlayer->getName());
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

void Client::update(float delta)
{
   Process::update(delta);

   mpWindow->update();

   if ( hasKeyMap() )
   {
      mpKeyMap->update();
   }

   graph.updateClient(delta);
}

void Client::render(float delta)
{
   glLoadIdentity ();
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glAlphaFunc (GL_GREATER, 0.1f);
   glEnable (GL_ALPHA_TEST);

   if ( mpWorldRenderer != NULL )
   {
      // set the sound of the player
      Object* pcontroler = graph.getControler();
      if ( pcontroler != NULL )
         mSoundManager.setPlayerPosition(pcontroler->getPosition());

      mpWorldRenderer->render(delta);
   }

   glDisable (GL_ALPHA_TEST);
}

void Client::display()
{
   mpWindow->display();
}

//---------------------------------------------
// - Get/set
//---------------------------------------------

INLINE Player& Client::getPlayer()
{
   ASSERT_PTR(mpPlayer);
   return *mpPlayer;
}

INLINE bool Client::hasKeyMap() const
{
   return mpKeyMap != NULL;
}

INLINE KeyMap& Client::getKeyMap()
{
   ASSERT_PTR(mpKeyMap)
   return *mpKeyMap;
}

INLINE void Client::setKeyMap(KeyMap* pkeymap)
{
   mpKeyMap = pkeymap;
   if ( mpKeyMap != NULL )
      mpKeyMap->setClient(*this);
}

void Client::setWindow(GameWindow* pwindow)
{
   delete mpWindow;
   mpWindow = pwindow;
}

//---------------------------------------------
// - Operations
//---------------------------------------------

bool Client::initOpenGL()
{
   bool success = OpenGL::initialize ();
   if ( success )
   {
      //const Color& color = mSettings.getClearColor();
      const Color color(75, 150, 230, 255);
      glClearColor(color.getRed(), color.getGreen(), color.getBlue(), 0.0f);

	   glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glShadeModel (GL_SMOOTH);
   }

	return success;
}

bool Client::loadWorld(const std::string& filename, const std::string& name)
{
   if ( Process::loadWorld(filename, name) )
   {
      graph.getWorld()->setReplica();

      mpWorldRenderer = graph.getWorld()->createRenderer();

      return true;
   }

   return false;
}

void Client::sendToServer(NetObject& object)
{
   conn.send(&object);
}

//---------------------------------------------
// - Events
//---------------------------------------------

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
      case namechangeEvent:
         {
            const NameChangeObjectEvent& namechangeevent = dynamic_cast<const NameChangeObjectEvent&>(event);
            handleNameChangeEvent(namechangeevent);
            break;
         }
   }
   
   return 0;
}

void Client::handleConnectReplyEvent(const ConnectReplyEvent& event)
{
   ASSERT_PTR(mpScript);

   switch ( event.getReply() )
   {
      case ConnectReplyEvent::eAccepted:
         {
            // run the onConnected script
            mpScript->run("onConnected");

            initialized = true;
            break;
         }
      case ConnectReplyEvent::eDenite:
         {
            // run the Client_onConnectionDenite script
            mpScript->addParam(event.getReason());
            mpScript->run("onConnectionDenite");
            break;
         }
   }
}

void Client::handleDisconnectEvent(const DisconnectEvent& event)
{
   // call the script
   mpScript->addParam(event.getId()+1);
   mpScript->run("onPlayerLeft");
}

void Client::handleJoinEvent(const JoinEvent& event)
{
   // run the onConnected script
   mpScript->addParam(event.getId()+1);
   mpScript->addParam(event.getPlayerName().c_str());
   mpScript->run("onJoined");
}

void Client::handleServerdownEvent()
{
   // server went down, run the onClientConnect script
   mpScript->run("onServerDown");
}

void Client::handleNewObjectEvent(const NewObjectEvent& event)
{
   SceneObject* pparent = graph.find(event.getParentId());
   if ( pparent == NULL )
   {
      UNREACHABLE("Parent of object not found.")
   }

   // a new object has been made on the server and 
   // is now also known on the client
   AutoPtr<SceneObject> obj = event.getObject();
   if ( obj->create(*pparent, event.getFileName()) )
   {
      obj.release();
   }
   else
   {
      // meh
      return;
   }

   if ( World::isWorld(*obj) )
   {
      World& world = dynamic_cast<World&>(*obj);

      if ( graph.hasWorld() )
         graph.getWorld()->destroy();

      mpWorldRenderer = world.createRenderer();
      mpPlayer->initialize(world);

      graph.setWorld((World*)obj.getPointer());

      // run the onWorldChanged script
      mpScript->run("onWorldChanged");
   }
   
   // remove the request
   Requests::iterator it = mRequests.find(obj->getId());
   if ( it != mRequests.end() )
      mRequests.erase(it);
}

void Client::handleDeleteObjectEvent(const DeleteObjectEvent& event)
{
   SceneObject* node = graph.find(event.getId());
   if ( node != NULL )
   {
      node->destroy();
      delete node;
   }
}

void Client::handleUpdateObjectEvent(const UpdateObjectEvent& event)
{
   SceneObject* pobject = graph.find(event.getId());
   if ( pobject == NULL )
   {
      if ( mRequests.find(event.getId()) == mRequests.end() )
      {
         // unknown object, must have been generated before the player entered the game
         RequestObjectEvent event(event.getId());
         getConnection()->send(&event);

         mRequests[event.getId()] = true;
      }
   }
   else
   {
      event.update(*pobject);
   }
}

void Client::handleNameChangeEvent(const NameChangeObjectEvent& event)
{
   SceneObject* pobject = graph.find(event.getId());
   ASSERT_PTR(pobject)

   pobject->setName(event.getName());
}

void Client::handleScriptEvent(const ScriptEvent& event)
{
   AutoPtr<BitStream> stream(event.getStream());

   // run the onClientConnect script
   mpScript->addParam("BitStream", stream.getPointer());
   mpScript->run("onScriptEvent");
}

// - Notifications

void Client::onWindowResized()
{
   // set the new opengl states
   glViewport(0, 0, mpWindow->getWidth(), mpWindow->getHeight());

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, mpWindow->getWidth(), mpWindow->getHeight(), 0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void Client::onWindowClosing()
{
}

void Client::onWindowClosed()
{
   setActive(false);
}
