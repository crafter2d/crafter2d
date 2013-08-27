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

#include "core/defines.h"
#include "core/smartptr/autoptr.h"
#include "core/log/log.h"
#include "core/math/color.h"
#include "core/math/xform.h"
#include "core/input/keyevent.h"
#include "core/input/mouseevent.h"
#include "core/graphics/device.h"
#include "core/graphics/devicefactory.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/viewport.h"
#include "core/system/platform.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "net/events/aggregateevent.h"
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

#include "clientcontentmanager.h"
#include "player.h"
#include "entity.h"
#include "actionmap.h"
#include "keymap.h"

using namespace Graphics;

Client::Client():
   Process(),
   mpWindowFactory(NULL),
   mpWindow(NULL),
   mpDevice(NULL),
   mpRenderContext(NULL),
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
   Log& log = Log::getInstance();
   log << "\n-- Initializing Sound --\n\n";

   // initialize the sound system
   mSoundManager.initialize();   

   return Process::create(classname);
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
      getWorld().updateClient(*mpRenderContext, delta);
   }
}

void Client::render(float delta)
{
   static const String sPaint = UTEXT("paint");

   mpRenderContext->clear();
   mpRenderContext->beginDraw();
   //mpRenderContext->setObjectMatrix(XForm::identity());
   //mpRenderContext->setWorldMatrix(XForm::identity());
   
   if ( mpWorldRenderer != NULL )
   {
      if ( mpPlayer->hasController() )
      {
         // set the sound of the player
         //Actor& controler = mpPlayer->getController();
         //mSoundManager.setPlayerPosition(controler.getPosition());
      }

      mpWorldRenderer->render(*mpRenderContext, delta);
   }

   mpRenderContext->endDraw();

   Variant arg((double)delta);
   mpScript->run(sPaint, 1, &arg);

   mpDevice->present();
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

      onWindowChanged();
   }
}

//---------------------------------------------
// - Operations
//---------------------------------------------

typedef Graphics::DeviceFactory* (*PFACTORY)();

bool Client::initDevice()
{
   Log::getInstance() << "\n-- Initializing Graphics --\n\n";

   void* pmodule = Platform::getInstance().loadModule(UTEXT("D3Dd.dll"));
   if ( pmodule == NULL )
   {
      return false;
   }

   PFACTORY pfactoryfnc = (PFACTORY)Platform::getInstance().getFunctionAddress(pmodule, UTEXT("getDeviceFactory"));
   if ( pfactoryfnc == NULL )
   {
      return false;
   }

   AutoPtr<DeviceFactory> pfactory = pfactoryfnc();
   if ( pfactory.hasPointer() )
   {
      static const Color color(75, 150, 230, 255);

      mpDevice = pfactory->createDevice();
      if ( !mpDevice->create(mpWindow->getHandle(), 800, 600) )
      {
         return false;
      }

      mpRenderContext = mpDevice->createRenderContext();
      mpRenderContext->setClearColor(color);
      mpRenderContext->initialize(*mpDevice);

      onWindowResized();
   }

	return true;
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
      case aggregateEvent:
         {
            const AggregateEvent& aevent = dynamic_cast<const AggregateEvent&>(event);
            const AggregateEvent::Events events = aevent.getEvents();
            for ( std::size_t index = 0; index < events.size(); index++ )
            {
               const NetEvent& netevent = *events[index];
               onNetEvent(client, netevent);
            }
            break;
         };
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
            Variant arg(mpScript->instantiate(UTEXT("engine.game.Player"), mpPlayer));
            mpScript->run(UTEXT("onConnected"), 1, &arg);

            initialized = true;
            break;
         }
      case ConnectReplyEvent::eDenite:
         {
            // run the Client_onConnectionDenite script
            Variant arg(event.getReason());
            mpScript->run(UTEXT("onConnectionDenite"), 1, &arg);
            break;
         }
   }
}

void Client::handleJoinEvent(const JoinEvent& event)
{
   // run the onConnected script
   Variant arg(event.getId() + 1);
   mpScript->run(UTEXT("onPlayerJoined"), 1, &arg);
}

void Client::handleDisconnectEvent(const DisconnectEvent& event)
{
   // call the script
   Variant arg(event.getId() + 1);
   mpScript->run(UTEXT("onPlayerLeft"), 1, &arg);
}

void Client::handleServerdownEvent()
{
   // server went down, run the onClientConnect script
   mpScript->run(UTEXT("onServerDown"));
}

void Client::handleWorldChangedEvent(const WorldChangedEvent& event)
{
   World* pworld = loadWorld(event.getFilename());
   if ( pworld == NULL )
   {
      // ee boehhh
      Log::getInstance().error("Failed to load world!");
      return;
   }
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

static const String sNetStream    = UTEXT("engine.net.NetStream");
static const String sScriptEvent  = UTEXT("onScriptEvent");
static const String sOnKeyEvent   = UTEXT("onKeyEvent");
static const String sOnMouseEvent = UTEXT("onMouseEvent");

void Client::handleScriptEvent(const ScriptEvent& event)
{
   DataStream& datastream = const_cast<DataStream&>(event.getStream());
   NetStream stream(datastream);

   // run the onClientConnect script
   Variant arg(mpScript->instantiate(sNetStream, &stream));
   mpScript->run(sScriptEvent, 1, &arg);
}

// - Notifications

void Client::notifyWorldChanged()
{
   // need a new content manager, the simulator, etc, have changed
   setContentManager(new ClientContentManager(*this));

   World& world = getWorld();
   world.initialize(*mpDevice);
   world.onViewportChanged(*mpRenderContext);

   mpWorldRenderer = world.createRenderer();
   mpPlayer->initialize(world);

   // run the onWorldChanged script
   Variant arg(mpScript->resolve(&world));
   mpScript->run(UTEXT("onWorldChanged"), 1, &arg);

   Process::notifyWorldChanged();
}

void Client::onWindowChanged()
{
   if ( hasWindow() )
   {
      mpWindow->addListener(mWindowListener);
      mpWindow->setKeyEventDispatcher(mKeyEventDispatcher);
      mpWindow->setMouseEventDispatcher(mMouseEventDispatcher);

      if ( !initDevice() )
      {
         //setActive(false);
      }
   }
}

void Client::onWindowResized()
{
   // set the new opengl states
   Graphics::Viewport viewport(0, 0, mpWindow->getWidth(), mpWindow->getHeight());

   mpRenderContext->setViewport(viewport);
   //mpRenderContext->setOrthoProjection();
   
   if ( hasWorld() )
   {
      getWorld().onViewportChanged(*mpRenderContext);
   }
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
   Variant args[2];
   args[0].setInt(event.getKey());
   args[1].setBool(event.getEventType() == KeyEvent::ePressed);
   mpScript->run(sOnKeyEvent, 2, args);
}

void Client::onMouseEvent(const MouseEvent& event)
{
   Variant args[4];
   args[0].setInt(event.getLocation().x());
   args[1].setInt(event.getLocation().y());
   args[2].setInt(event.getButtons());
   args[3].setInt(event.getEventType());
   mpScript->run(sOnMouseEvent, 4, args);
}

