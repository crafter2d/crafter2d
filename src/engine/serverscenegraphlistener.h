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

#ifndef SERVER_SCENEGRAPH_LISTENER_H_
#define SERVER_SCENEGRAPH_LISTENER_H_

#include "scenegraphlistener.h"

class Server;

class ServerSceneGraphListener : public SceneGraphListener
{
public:
   explicit ServerSceneGraphListener(Server& server);
   virtual ~ServerSceneGraphListener();

 // notifications
   virtual void notifyObjectAdded(const SceneObject& object);
   virtual void notifyObjectRemoved(const SceneObject& object);
   virtual void notifyObjectNameChanged(const SceneObject& object);

private:
   Server& mServer;
};

#endif // SCENEGRAPH_LISTENER_H
