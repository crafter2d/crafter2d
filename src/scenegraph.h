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
#ifndef _SCENEGRAPH_H__jengine
#define _SCENEGRAPH_H__jengine

#include <SDL/SDL.h>
#include <list>
#include <map>

#include "defines.h"
#include "hashtable.h"
#include "sceneobject.h"
#include "sceneroot.h"

class Object;
class World;
class DirtySet;
class SceneObject;
class SceneGraph;
class NetConnection;
class NodeVisitor;
class Simulator;
class TiXmlDocument;

/**
@author Jeroen Broekhuizen

@section scenegraph_intro Using the SceneGraph

Every object that is drawn on screen is stored in this scene graph.
*/
class SceneGraph
{
public:
   typedef std::map<std::string, SceneObject*> ObjectMap;

                     SceneGraph();
                     ~SceneGraph();

  // get/set
   bool           hasSimulator() const;
   Simulator&     getSimulator();
   void           setSimulator(Simulator* psimulator);

   void              update(DirtySet& dirtyset, float delta);
   void              updateClient(float delta);
   void              draw();

   void              addObject(SceneObject* obj);
   void              removeObject(const std::string& name);
   void              removeAll();

   SceneObject*      find(const std::string& node);

   void              setNotify(bool notify = true);
   void              setControler(Object* c);
   void              setWorld(World* w);

   Object*           getControler();
   bool              getNotify();
   World*            getWorld();
   SceneObject&      getRoot();

  // notifications
   void notifyNameChanged(SceneObject& object, const std::string& oldname);

protected:

   ObjectMap objects;
   SceneRoot root;
   Object* controler;
   World* world;
   Simulator* mpSimulator;
   bool notifyClients;
};

#ifdef JENGINE_INLINE
#  include "scenegraph.inl"
#endif

#endif
