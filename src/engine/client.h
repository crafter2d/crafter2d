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

#include "window/gamewindow.h"
#include "clientgamewindowlistener.h"
#include "idmanager.h"
#include "object.h"
#include "process.h"
#include "sound.h"

class ConnectReplyEvent;
class DisconnectEvent;
class JoinEvent;
class NewObjectEvent;
class DeleteObjectEvent;
class UpdateObjectEvent;
class NameChangeObjectEvent;
class ScriptEvent;

class Input;
class GameWindow;
class KeyMap;
class WorldRenderer;
class Player;

/// \brief The client side process. It handles user input and rendering of the server objects.
///
/// The Client class implements the client process of the Client/Server model this engine is build
/// upon.
class ENGINE_API Client: public Process
{
public:
   typedef std::map<Id, bool> Requests;

                  Client();
   virtual        ~Client();

   virtual bool   create(const std::string& name);
   virtual bool   destroy();
   virtual void   update(float delta);

           void   render(float delta);
           void   display();

 // get/set
   Player&        getPlayer();

   bool           hasKeyMap() const;
   KeyMap&        getKeyMap();
   void           setKeyMap(KeyMap* pkeymap);

   bool           hasInput() const;
   Input&         getInput();
   void           setInput(Input& input);

   void           setWindow(GameWindow* pwindow);

 // operations
   virtual bool   loadWorld(const std::string& filename, const std::string& name);

   bool           connect(const char* server, int port, const char* name);
   void           disconnect();

   void           sendToServer(NetObject& object);

 // notifications
   void onWindowResized();
   void onWindowClosing();
   void onWindowClosed();

 // network event callback
   virtual int    onClientEvent(int client, const NetEvent& event);

private:
 // initialization
   bool initOpenGL();

 // event handlers
   void  handleConnectReplyEvent(const ConnectReplyEvent& event);
   void  handleDisconnectEvent(const DisconnectEvent& event);
   void  handleJoinEvent(const JoinEvent& event);
   void  handleServerdownEvent();

   void  handleNewObjectEvent(const NewObjectEvent& event);
   void  handleDeleteObjectEvent(const DeleteObjectEvent& event);
   void  handleUpdateObjectEvent(const UpdateObjectEvent& event);
   void  handleNameChangeEvent(const NameChangeObjectEvent& event);
   void  handleScriptEvent(const ScriptEvent& event);

   GameWindow*                mpWindow;
   ClientGameWindowListener   mWindowListener;
   SoundManager               mSoundManager;
   WorldRenderer*             mpWorldRenderer;
   Player*                    mpPlayer;
   KeyMap*                    mpKeyMap;
   Input*                     mpInput;
   Requests                   mRequests;
};

#ifdef JENGINE_INLINE
#  include "client.inl"
#endif

#endif
