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

#include <tinyxml.h>

#include "net/newobjectevent.h"
#include "net/events/deleteobjectevent.h"
#include "net/events/namechangeobjectevent.h"

#include "physics/simulator.h"
#include "physics/simulationfiller.h"

#include "world/world.h"

#include "game.h"
#include "object.h"
#include "console.h"
#include "nodevisitor.h"

SceneGraph::SceneGraph():
   objects(),
   root(),
   controler(NULL),
   world(NULL),
   mpSimulator(NULL),
   notifyClients(false)
{
   objects["root"] = &root;
}

SceneGraph::~SceneGraph()
{
   world = 0;
   controler = 0;
}

void SceneGraph::setWorld(World* w)
{
   world = w;
   world->setName("world");
   if (!w->isReplica() || !objects[w->getName()])
      root.add(world);
}

void SceneGraph::update(DirtySet& dirtyset, float delta)
{
   if ( hasSimulator() )
      getSimulator().run(delta);

   root.update(dirtyset, delta);
}

void SceneGraph::updateClient(float delta)
{
   root.updateClient(delta);
}

void SceneGraph::draw()
{
   root.draw();
}

SceneObject* SceneGraph::find(const std::string& node)
{
   if (objects.find(node) == objects.end())
      return 0;
   else
      return objects[node];
}

/// \fn SceneGraph::addObject(SceneObject* obj, const char* name)
/// \brief Inserts the object in the hashtable.
void SceneGraph::addObject(SceneObject* obj)
{
   // add object to the hash table
   objects[obj->getName()] = obj;

   if ( notifyClients )
   {
      NewObjectEvent event(*obj);
      Game::getInstance().getServer().sendToAllClients(event);

      if ( hasSimulator() )
         SimulationFiller::add(getSimulator(), *obj);
   }
}

/// \fn removeObject(const char* name)
/// \brief Remove a scene node from the scene graph (optionally inform clients about removal).
void SceneGraph::removeObject(const std::string& name)
{
   ObjectMap::iterator it = objects.find(name);
   if (it != objects.end())
   {
      objects.erase(it);

      if ( notifyClients )
      {
         DeleteObjectEvent event(name);
         Game::getInstance().getServer().sendToAllClients(event);
      }
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

void SceneGraph::notifyNameChanged(SceneObject& object, const std::string& oldname)
{
   ObjectMap::iterator it = objects.find(oldname);
   if ( it != objects.end() )
   {
      objects.erase(it);

      objects[object.getName()] = &object;

      if ( notifyClients )
      {
         NameChangeObjectEvent event(oldname, object.getName());
         Game::getInstance().getServer().sendToAllClients(event);
      }
   }
}
