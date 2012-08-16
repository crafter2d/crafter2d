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

#include <GL/GLee.h>
#include <GL/glu.h>

#include "core/defines.h"
#include "core/smartptr/autoptr.h"
#include "core/log/log.h"
#include "core/math/color.h"
#include "core/input/keyevent.h"
#include "core/input/mouseevent.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "net/events/newobjectevent.h"
#include "net/events/connectevent.h"
#include "net/events/connectreplyevent.h"
#include "net/events/deleteobjectevent.h"
#include "net/events/disconnectevent.h"
#include "net/events/joinevent.h"
#include "net/events/updateobjectevent.h"
#include "net/events/requestobjectevent.h"
#include "net/events/worldchangedevent.h"
#include "net/events/scriptevent.h"
#include "net/netstream.h"

#include "world/world.h"
#include "world/worldrenderer.h"

#include "player.h"
#include "entity.h"
#include "actionmap.h"
#include "keymap.h"
#include "opengl.h"

Client::Client():
   Process(),
   mpWindow(NULL),
   mWindowListener(*this),
   mKeyEventDispatcher(*this),
   mMouseEventDispatcher(*this),
   mSoundManager(),
   mpWorldRenderer(NULL),
   mpPlayer(NULL),
   mpKeyMap(NULL),
   mpInput(NULL),
   mRequests(),
   mServerId(-1)
{
}

Client::~Client()
{
}

// - Creation

bool Client::create(const String& classname)
{
   bool success = Process::create(classname);
   if ( success )
   {
      ASSERT_PTR(mpWindow);

      mpWindow->addListener(mWindowListener);
      mpWindow->setKeyEventDispatcher(mKeyEventDispatcher);
      mpWindow->setMouseEventDispatcher(mMouseEventDispatcher);

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
   mSoundManager.destroy();

   mpWindow->destroy();
   delete mpWindow;
   mpWindow = NULL;

   delete mpPlayer;
   mpPlayer = NULL;

   delete mpKeyMap;
   mpKeyMap = NULL;

   delete mpWorldRenderer;
   mpWorldRenderer = NULL;

   return Process::destroy();
}

bool Client::connect(const String& server, int port)
{
   // setup connection to the server
   mServerId = conn.connect(server, port);
   if ( mServerId == -1 )
   {
      return false;
   }

   mpPlayer = new Player();

   // send login command
   ConnectEvent event;
   conn.send(mServerId, event);
   return true;
}

void Client::disconnect()
{
   if ( conn.isConnected() )
   {
      DisconnectEvent event;
      conn.send(mServerId, event);
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

   if ( hasWorld() )
   {
      getWorld().updateClient(delta);
   }
}

void Client::render(float delta)
{
   glLoadIdentity ();
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glAlphaFunc (GL_GREATER, 0.1f);
   glEnable (GL_ALPHA_TEST);

   if ( mpWorldRenderer != NULL )
   {
      if ( mpPlayer->hasController() )
      {
         // set the sound of the player
         Actor& controler = mpPlayer->getController();
         mSoundManager.setPlayerPosition(controler.getPosition());
      }

      mpWorldRenderer->render(delta);
   }

   glDisable (GL_ALPHA_TEST);

   mpWindow->display();
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
   if ( mpWindow != pwindow )
   {
      delete mpWindow;
      mpWindow = pwindow;
   }
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

bool Client::loadWorld(const String& filename, const String& name)
{
   World* pworld = getContentManager().load(filename);
   if ( pworld != NULL )
   {
      mpWorldRenderer = pworld->createRenderer();
   }

   return pworld != NULL;
}

void Client::sendToServer(NetObject& object)
{
   conn.send(mServerId, object);
}

//---------------------------------------------
// - Events
//---------------------------------------------

void Client::onNetEvent(int client, const NetEvent& event)
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
      case worldchangedEvent:
         {
            const WorldChangedEvent& worldchangedevent = static_cast<const WorldChangedEvent&>(event);
            handleWorldChangedEvent(worldchangedevent);
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
}

void Client::handleConnectReplyEvent(const ConnectReplyEvent& event)
{
   ASSERT_PTR(mpScript);

   switch ( event.getReply() )
   {
      case ConnectReplyEvent::eAccepted:
         {
            // run the onConnected script
            mpScript->addParam("engine.game.Player", mpPlayer);
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

void Client::handleJoinEvent(const JoinEvent& event)
{
   // run the onConnected script
   mpScript->addParam(event.getId()+1);
   mpScript->run("onPlayerJoined");
}

void Client::handleDisconnectEvent(const DisconnectEvent& event)
{
   // call the script
   mpScript->addParam(event.getId()+1);
   mpScript->run("onPlayerLeft");
}

void Client::handleServerdownEvent()
{
   // server went down, run the onClientConnect script
   mpScript->run("onServerDown");
}

void Client::handleWorldChangedEvent(const WorldChangedEvent& event)
{
   World* pworld = getContentManager().load(event.getFilename());
   if ( pworld == NULL )
   {
      // ee boehhh
      return;
   }

   setWorld(pworld);

   mpWorldRenderer = pworld->createRenderer();
   mpPlayer->initialize(*pworld);

   // run the onWorldChanged script
   mpScript->addParam("engine.game.World", pworld);
   mpScript->run("onWorldChanged");
}

void Client::handleNewObjectEvent(const NewObjectEvent& event)
{
   ASSERT(hasWorld());

   // a new object has been made on the server and is now also known on the client

   AutoPtr<Entity> entity = getContentManager().loadEntity(event.getFileName());
   if ( !entity.hasPointer() )
   {
      UNREACHABLE("Could not create the entity!");
   }

   entity->setId(event.getId());
   entity->setReplica();

   if ( event.getParentId() != IdManager::invalidId )
   {
      Entity* pparent = getWorld().findEntity(event.getParentId());
      if ( pparent == NULL )
      {
         UNREACHABLE("Could not find the parent!!");
      }
      pparent->add(*entity);
   }

   // remove the request
   Requests::iterator it = mRequests.find(entity->getId());
   if ( it != mRequests.end() )
      mRequests.erase(it);

   // add the entity to the world
   getWorld().addEntity(entity.release());
}

void Client::handleDeleteObjectEvent(const DeleteObjectEvent& event)
{
   getWorld().removeEntity(event.getId());
}

void Client::handleUpdateObjectEvent(const UpdateObjectEvent& event)
{
   Entity* pentity = getWorld().findEntity(event.getId());
   if ( pentity == NULL )
   {
      if ( mRequests.find(event.getId()) == mRequests.end() )
      {
         // unknown object, must have been generated before the player entered the game
         RequestObjectEvent event(event.getId());
         conn.send(mServerId, event);

         mRequests[event.getId()] = true;
      }
   }
   else
   {
      event.update(*pentity);
   }
}

void Client::handleScriptEvent(const ScriptEvent& event)
{
   DataStream& datastream = const_cast<DataStream&>(event.getStream());
   NetStream stream(datastream);

   // run the onClientConnect script
   mpScript->addParam("engine.net.NetStream", &stream);
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

void Client::onKeyEvent(const KeyEvent& event)
{
   mpScript->addParam(event.getKey());
   mpScript->addParam(event.getEventType() == KeyEvent::ePressed);
   mpScript->run("onKeyEvent");
}

void Client::onMouseEvent(const MouseEvent& event)
{
   mpScript->addParam(event.getLocation().x());
   mpScript->addParam(event.getLocation().y());
   mpScript->addParam(event.getButtons());
   mpScript->addParam(event.getEventType());
   mpScript->run("onMouseEvent");
}

