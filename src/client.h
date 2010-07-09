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
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <map>
#include "object.h"
#include "process.h"

class ConnectReplyEvent;
class DisconnectEvent;
class JoinEvent;
class NewObjectEvent;
class DeleteObjectEvent;
class UpdateObjectEvent;
class NameChangeObjectEvent;
class ScriptEvent;

class KeyMap;
class WorldRenderer;
class Player;

/// @author Jeroen Broekhuizen
///
/// Implement the client process of the Client/Server model.
class Client: public Process
{
public:
   typedef std::map<const char*, bool> Requests;

                  Client();
   virtual        ~Client();

   virtual bool   destroy();
   virtual void   update(float delta);
           void   render(float delta);

 // get/set
   Player&        getPlayer();

   bool           hasKeyMap() const;
   KeyMap&        getKeyMap();
   void           setKeyMap(KeyMap* pkeymap);

 // operations
   virtual bool loadWorld(const std::string& filename, const std::string& name);

   bool         connect(const char* server, int port, const char* name);
   void         disconnect();

   virtual int  onClientEvent(int client, const NetEvent& event);

private:
   void  handleConnectReplyEvent(const ConnectReplyEvent& event);
   void  handleDisconnectEvent(const DisconnectEvent& event);
   void  handleJoinEvent(const JoinEvent& event);
   void  handleServerdownEvent();

   void  handleNewObjectEvent(const NewObjectEvent& event);
   void  handleDeleteObjectEvent(const DeleteObjectEvent& event);
   void  handleUpdateObjectEvent(const UpdateObjectEvent& event);
   void  handleNameChangeEvent(const NameChangeObjectEvent& event);
   void  handleScriptEvent(const ScriptEvent& event);

   WorldRenderer* mpWorldRenderer;
   Player*        mpPlayer;
   KeyMap*        mpKeyMap;
   Requests       requests;
};

#endif
