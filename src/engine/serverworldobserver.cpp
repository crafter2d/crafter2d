/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "serverworldobserver.h"

#include "engine/net/events/newobjectevent.h"
#include "engine/net/events/deleteobjectevent.h"

#include "server.h"

ServerWorldObserver::ServerWorldObserver(Server& server):
   mServer(server)
{
}
 
// - Notifications

void ServerWorldObserver::notifyEntityAdded(const Entity& entity)
{
   NewObjectEvent event(entity);
   mServer.sendToAllClients(event);
}
   
void ServerWorldObserver::notifyEntityRemoved(const Entity& entity)
{
   DeleteObjectEvent event(entity);
   mServer.sendToAllClients(event);
}
