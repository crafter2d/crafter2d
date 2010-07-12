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
#include "scenegraph.h"
#ifndef JENGINE_INLINE
#  include "scenegraph.inl"
#endif

#include "net/newobjectevent.h"
#include "net/events/deleteobjectevent.h"
#include "net/events/namechangeobjectevent.h"

#include "world/world.h"

#include "game.h"
#include "object.h"
#include "console.h"

SceneGraph::SceneGraph():
   mObjects(),
   mRoot(),
   mpControler(NULL),
   mpWorld(NULL),
   mNotifyClients(false)
{
   mObjects[mRoot.getId()] = &mRoot;
}

SceneGraph::~SceneGraph()
{
   mpControler = NULL;
   mpWorld     = NULL;
}

void SceneGraph::setWorld(World* w)
{
   mpWorld = w;
   mpWorld->setName("world");

   if ( !mRoot.contains(*mpWorld) )
      mRoot.add(mpWorld);
}

void SceneGraph::update(DirtySet& dirtyset, float delta)
{
   mRoot.update(dirtyset, delta);
}

void SceneGraph::updateClient(float delta)
{
   mRoot.updateClient(delta);
}

void SceneGraph::draw()
{
   mRoot.draw();
}

SceneObject* SceneGraph::find(const Id& id)
{
   ObjectMap::iterator it = mObjects.find(id);
   return (it != mObjects.end()) ? it->second : NULL;
}

/// \fn SceneGraph::addObject(SceneObject* obj, const char* name)
/// \brief Inserts the object in the hashtable.
void SceneGraph::addObject(SceneObject* obj)
{
   // add object to the hash table
   mObjects[obj->getId()] = obj;

   if ( mNotifyClients )
   {
      NewObjectEvent event(*obj);
      Game::getInstance().getServer().sendToAllClients(event);
   }
}

/// \fn removeObject(const char* name)
/// \brief Remove a scene node from the scene graph (optionally inform clients about removal).
void SceneGraph::removeObject(const SceneObject& object)
{
   ObjectMap::iterator it = mObjects.find(object.getId());
   if (it != mObjects.end())
   {
      mObjects.erase(it);

      if ( mNotifyClients )
      {
         DeleteObjectEvent event(object);
         Game::getInstance().getServer().sendToAllClients(event);
      }
   }
}

/// \fn SceneGraph::removeAll()
/// \fn Brief removes all scene graph nodes.
void SceneGraph::removeAll()
{
   mRoot.removeAll();

   mObjects.clear();
   mObjects[mRoot.getId()] = &mRoot;

   mpControler = NULL;
   mpWorld     = NULL;
}

void SceneGraph::notifyNameChanged(const SceneObject& object)
{
   ObjectMap::iterator it = mObjects.find(object.getId());
   if ( it != mObjects.end() )
   {
      mObjects.erase(it);

      if ( mNotifyClients )
      {
         NameChangeObjectEvent event(object);
         Game::getInstance().getServer().sendToAllClients(event);
      }
   }
}
