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
#ifndef _SERVER_H_
#define _SERVER_H_

#include <map>

#include "net/netevent.h"

#include "process.h"
#include "serverscenegraphlistener.h"

class ConnectEvent;
class ViewportEvent;
class World;
class Player;

class JoinEvent: public NetEvent
{
public:
   DEFINE_REPLICATABLE(JoinInfo)

   JoinEvent();
   JoinEvent(int id, const std::string& name);

   int getId() const { return id; }
   const char* getPlayerName() const { return playerName.c_str(); }

   virtual void pack(BitStream& stream) const;
   virtual void unpack(BitStream& stream);

private:
   std::string playerName;
   int id;
};

class DisconnectEvent: public NetEvent
{
public:
   DEFINE_REPLICATABLE(DisconnectInfo)

            DisconnectEvent();
   explicit DisconnectEvent(int id);

   int getId() const { return _id; }

   virtual void pack(BitStream& stream) const;
   virtual void unpack(BitStream& stream);

private:
   int         _id;
};

/// @author Jeroen Broekhuizen
///
/// Implement the server process of the Client/Server model.
class Server: public Process
{
public:
   typedef std::map<Uint32, Player*> ClientMap;

                  Server();
   virtual        ~Server();

   virtual bool   create();
   virtual bool   destroy();
   void           shutdown();

   void           addPlayer(int client, Player* player);

   void           sendToAllClients(BitStream& stream);
   void           sendToAllClients(NetObject& object);
   void           sendScriptEventToAllClients(BitStream* stream);

   virtual void   update(float delta);

   virtual bool   loadWorld(const std::string& filename, const std::string& name);

 // network events
   virtual int    allowNewConnection();
   virtual int    onClientEvent(int client, const NetEvent& event);

protected:
   void           sendToActiveClient(NetObject& stream);

   void handleConnectEvent(const ConnectEvent& event);
   void handleViewportEvent(const ViewportEvent& event);

   ClientMap                clients;
   int                      mActiveClient;
   ServerSceneGraphListener mGraphListener;
};

#endif
