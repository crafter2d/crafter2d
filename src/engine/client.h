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
#ifndef CLIENT_H_
#define CLIENT_H_

#include "engine_base.h"

#include <map>

#include "core/entity/idmanager.h"
#include "core/graphics/viewport.h"
#include "core/string/string.h"
#include "core/sound/soundmanager.h"

#include "clientgamewindowlistener.h"
#include "clientkeyeventdispatcher.h"
#include "clientmouseeventdispatcher.h"
#include "process.h"

namespace Graphics
{
   class Device;
   class Font;
   class RenderContext;
};

namespace Input
{
   class InputDevice;
   class KeyEvent;
   class MouseEvent;
};

class ConnectReplyEvent;
class DisconnectEvent;
class JoinEvent;
class NewObjectEvent;
class DeleteObjectEvent;
class UpdateObjectEvent;
class NameChangeObjectEvent;
class ScriptEvent;
class WorldChangedEvent;

class Driver;
class Entity;
class GameWindow;
class GameWindowFactory;
class WorldRenderer;
class Player;

namespace c2d
{
   class KeyMap;

   /// \brief The client side process. It handles user input and rendering of the server objects.
   ///
   /// The Client class implements the client process of the Client/Server model this engine is build
   /// upon.
   class ENGINE_API Client : public Process
   {
   public:
      typedef std::map<Id, bool> Requests;

      Client();
      virtual        ~Client();

      virtual bool   destroy();
      virtual void   update(float delta);

      void   render(float delta);
      void   display();

      // get/set
      Player&        getPlayer();

      bool           hasKeyMap() const;
      KeyMap&        getKeyMap();
      void           setKeyMap(KeyMap* pkeymap);

      GameWindowFactory& getWindowFactory();
      void               setWindowFactory(GameWindowFactory& factory);

      bool               hasWindow() const;
      GameWindow&        getWindow();
      void               setWindow(GameWindow* pwindow);

      Graphics::Device&        getDevice();
      Graphics::RenderContext& getRenderContext();

      Input::InputDevice&  getInput();

      Graphics::Viewport& getViewport() { return mViewport; }

      // operations
      bool           connect(const String& server, int port);
      void           disconnect();

      void           sendToServer(NetObject& object);

      // notifications
      void onWindowChanged();
      void onWindowResized();
      void onWindowClosing();
      void onWindowClosed();

      void onKeyEvent(const Input::KeyEvent& event);
      void onMouseEvent(const Input::MouseEvent& event);

      // network event callback
      virtual void onNetEvent(int client, const NetEvent& event);

   protected:
      // notifications
      virtual void notifyWorldChanged();

   private:
      friend class ClientMouseEventListener;
      friend class ClientKeyEventListener;

      // initialization
      bool initDevice();
      bool initGraphics();
      bool initInput();
      bool initSound();

      // event handlers
      void  handleConnectReplyEvent(const ConnectReplyEvent& event);
      void  handleDisconnectEvent(const DisconnectEvent& event);
      void  handleJoinEvent(const JoinEvent& event);
      void  handleServerdownEvent();

      void  handleWorldChangedEvent(const WorldChangedEvent& event);
      void  handleNewObjectEvent(const NewObjectEvent& event);
      void  handleDeleteObjectEvent(const DeleteObjectEvent& event);
      void  handleUpdateObjectEvent(const UpdateObjectEvent& event);
      void  handleScriptEvent(const ScriptEvent& event);

    // data
      GameWindowFactory*         mpWindowFactory;
      GameWindow*                mpWindow;
      Graphics::Device*          mpDevice;
      Graphics::RenderContext*   mpRenderContext;
      Input::InputDevice*        mpInputDevice;
      ClientGameWindowListener   mWindowListener;
      ClientKeyEventDispatcher   mKeyEventDispatcher;
      ClientMouseEventDispatcher mMouseEventDispatcher;
      SoundManager*              mpSoundManager;
      Sound*                     mpBackgroundMusic;
      WorldRenderer*             mpWorldRenderer;
      Entity*                    mpParticleEntity;
      Player*                    mpPlayer;
      KeyMap*                    mpKeyMap;
      Graphics::Font*            mpFont;
      Graphics::Viewport         mViewport;
      String                     mFpsMsg;
      Requests                   mRequests;
      int                        mServerId;
   };

}

#ifdef JENGINE_INLINE
#  include "client.inl"
#endif

#endif
