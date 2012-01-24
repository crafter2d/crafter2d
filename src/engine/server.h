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

#include "engine_base.h"

#include <map>

#include "net/netevent.h"

#include "serverworldobserver.h"
#include "process.h"

class ConnectEvent;
class ViewportEvent;
class World;
class Player;

/// @author Jeroen Broekhuizen
///
/// Implement the server process of the Client/Server model.
class ENGINE_API Server: public Process
{
public:
   typedef std::map<uint, Player*> ClientMap;

                  Server();
   virtual        ~Server();

   virtual bool   create(const std::string& name);
   virtual bool   destroy();

   void           listen(int port);
   void           shutdown();

   void           addPlayer(int client, Player* player);

   void           sendToAllClients(BitStream& stream);
   void           sendToAllClients(NetObject& object);
   void           sendScriptEventToAllClients(BitStream* stream);

   virtual void   update(float delta);

 // network events
   virtual int    allowNewConnection();
   virtual int    onClientEvent(int client, const NetEvent& event);

protected:
 // notifications
   virtual void notifyWorldChanged();

 // networking
   void sendToActiveClient(NetObject& stream);

   void handleConnectEvent(const ConnectEvent& event);
   void handleViewportEvent(const ViewportEvent& event);

private:

   ClientMap                clients;
   ServerWorldObserver      mWorldObserver;
   int                      mActiveClient;
};

#endif
