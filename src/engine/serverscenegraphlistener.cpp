/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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

#include "serverscenegraphlistener.h"

#include "net/newobjectevent.h"
#include "net/events/deleteobjectevent.h"
#include "net/events/namechangeobjectevent.h"

#include "server.h"

ServerSceneGraphListener::ServerSceneGraphListener(Server& server):
   mServer(server)
{
}

ServerSceneGraphListener::~ServerSceneGraphListener()
{
}

//-----------------------------------------
// - Notifications
//-----------------------------------------

void ServerSceneGraphListener::notifyObjectAdded(const SceneObject& object)
{
   NewObjectEvent event(object);
   mServer.sendToAllClients(event);
}

void ServerSceneGraphListener::notifyObjectRemoved(const SceneObject& object)
{
   DeleteObjectEvent event(object);
   mServer.sendToAllClients(event);
}

void ServerSceneGraphListener::notifyObjectNameChanged(const SceneObject& object)
{
   NameChangeObjectEvent event(object);
   mServer.sendToAllClients(event);
}
