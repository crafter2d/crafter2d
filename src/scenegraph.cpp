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
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "scenegraph.h"
#ifndef JENGINE_INLINE
#  include "scenegraph.inl"
#endif

#include <tinyxml.h>

#include "net/netconnection.h"
#include "net/newobjectevent.h"

#include "world/world.h"

#include "game.h"
#include "object.h"
#include "console.h"
#include "nodevisitor.h"

SceneGraph::SceneGraph():
   conn(0),
   notifyClients(false),
   controler(0),
   world(0)
{
   root.setName("root");
   objects["root"] = &root;
}

SceneGraph::~SceneGraph()
{
   world = 0;
   controler = 0;
}

void SceneGraph::setControler(Object* c)
{
   controler = c;
}

Object* SceneGraph::getControler()
{
   return controler;
}

void SceneGraph::setWorld(World* w)
{
   world = w;
   if (!w->isReplica() || !objects[w->getName()])
      root.add (world);
}

void SceneGraph::update(Uint32 tick)
{
   root.update(tick);
}

void SceneGraph::draw()
{
   root.draw();
}

SceneObject* SceneGraph::find(const char* node)
{
   if (objects.find(node) == objects.end())
      return 0;
   else
      return objects[node];
}

/// \fn SceneGraph::addObject(SceneObject* obj, const char* name)
/// \brief Inserts the object in the hashtable.
void SceneGraph::addObject(SceneObject* obj, const char* name)
{
   // add object to the hash table
   objects[name] = obj;

   if (notifyClients)
   {
      BitStream stream;
      NewObjectEvent event(*obj);
      stream << &event;

      // notify the clients that a new object is added
      Game::getInstance().getServer().sendToAllClients(stream);
   }
}

/// \fn removeObject(const char* name)
/// \brief Remove a scene node from the scene graph (optionally inform clients about removal).
void SceneGraph::removeObject(const char* name)
{
   assert(name && "A scene node must have a name!");

   ObjectMap::iterator it = objects.find(name);
   if (it != objects.end()) {
      objects.erase(it);
   }

   if (notifyClients) {
      BitStream stream;
      NetEvent event(delobjectEvent);

      // let the clients know that the object has been removed
      stream << &event << name;
      Game::getInstance().getServer().sendToAllClients(stream);
   }
}

/// \fn SceneGraph::removeAll()
/// \fn Brief removes all scene graph nodes.
void SceneGraph::removeAll()
{
   root.removeAll();

   objects.clear();
   objects["root"] = &root;

   controler = 0;
   world = 0;
}

/// \fn transmitObjects(const char* key, void* data, void* param)
/// \brief Called by enumerate to transmit the objects changes to the client.
void SceneGraph::transmitObject(const char* key, void* data) const
{
   SceneObject* obj = (SceneObject*)data;
   if (obj->isDirty() && !obj->isStatic()) {
      BitStream stream;
      NetEvent event(updobjectEvent);

      // fill it in
      stream << &event;
      stream << key;
      obj->pack(stream);

      // send the package
      conn->send(&stream);
   }
}

/// \fn SceneGraph::transmitChanges(NetConnection* conn)
/// \brief Transmit all objects who are have their dirty flag set.
void SceneGraph::transmitChanges()
{
   //objects.enumerate(transmitObjects, this);
   ObjectMap::iterator it = objects.begin();
   for ( ; it != objects.end(); ++it) {
      transmitObject((*it).first, (*it).second);
   }
}
