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

#include "world/world.h"

#include "object.h"

SceneGraph::SceneGraph(Process& process):
   mProcess(process),
   mObjects(),
   mRoot(*this),
   mpListener(NULL),
   mpControler(NULL),
   mpWorld(NULL)
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

/// \fn SceneGraph::addObject(SceneObject& object)
/// \brief Inserts the object in the hashtable.
void SceneGraph::addObject(SceneObject& object)
{
   // add object to the hash table
   mObjects[object.getId()] = &object;

   if ( mpListener != NULL )
   {
      mpListener->notifyObjectAdded(object);
   }
}

/// \fn removeObject(const SceneObject& object)
/// \brief Remove a scene node from the scene graph (optionally inform clients about removal).
void SceneGraph::removeObject(const SceneObject& object)
{
   ObjectMap::iterator it = mObjects.find(object.getId());
   if (it != mObjects.end())
   {
      mObjects.erase(it);

      if ( mpListener != NULL )
      {
         mpListener->notifyObjectRemoved(object);
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
      if ( mpListener != NULL )
      {
         mpListener->notifyObjectNameChanged(object);
      }
   }
}
