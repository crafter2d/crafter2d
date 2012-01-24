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
#ifndef WORLD_H_
#define WORLD_H_

#include <string>
#include <vector>
#include <map>

#include "core/math/vector.h"

#include "engine/idmanager.h"
#include "engine/worldobserver.h"
#include "engine/script/script.h"

#include "bounds.h"
#include "layertype.h"
#include "worldsimulatorlistener.h"

class BitStream;
class DirtySet;
class Entity;
class Layer;
class Bound;
class NodeVisitor;
class SimulationFactory;
class Simulator;
class WorldRenderer;

/**
@author Jeroen Broekhuizen
\brief Represents a complete world consisting off unlimited layers.

In JEngine a world is a collection of one or more layers (Layer objects) displayed
in a predefined order. Think of a background, the walk layer and a foreground (which
a monster can hide behind). This class performs all these tasks including loading,
rendering and scrolling.

@section world_scroll Let the world scroll
By default the world will scroll the layers based on the position of the mouse cursor.
When the cursor is near a window border the world will be scrolled in the direction of
the side the cursor is at. So, keeping the cursor at the left side will scroll the world
to the left. Keeping the cursor in a cursor will scroll the cursor both in the x and y
axis.

By using the World::setScrollMode function you can change the behaviour of the engine.
When using the FollowObject modus you must also set the object to follow with the
setFollowObject function.
*/
class World
{
public:
   enum FollowMode { FollowObject=0, FollowMouse=1, NoFollow=2 };

   typedef std::vector<Layer*> Layers;
   typedef std::vector<WorldObserver*> Observers;
   typedef std::map<Id, Entity*> EntityMap;
   
   World();
   ~World();

   void  initialize();
   void  destroy();
   bool  save();

 // get/set
   void           setAutoFollow(bool enabled=true);
   void           setFollowObject(Actor* obj);
   void           setScrollMode(int fm);
   void           setFollowBorderWidth(int width);
   void           setFollowBorders(int left, int right, int top, int bottom);
   void           setObjectLayer(int layer);

   const std::string&   getName() const;
   void                 setName(const std::string& name);

   const std::string&   getFilename() const;
   void                 setFilename(const std::string& filename);

   const Bound&   getBound(int index) const;
         Bound&   getBound(int index);
         int      getBoundCount() const;

   const Bounds&  getBounds() const;

   int            getLayerCount() const;
   Layer*         getLayer(int index);
   const Layer*   getLayer(int index) const;
   int            getObjectLayer() const;
   
   LayerType      getLayerType() const;
   void           setLayerType(int type);

   SimulationFactory& getSimulationFactory();
   void               setSimulationFactory(SimulationFactory& factory);
   Simulator&         getSimulator();

   bool     hasScript() const;
   Script&  getScript();
   void     setScript(Script* pscript);

 // observer interface
   void  attach(WorldObserver& observer);
   void  detach(WorldObserver& observer);

 // visitor interface
   void   accept(NodeVisitor& nv);

 // update & drawing
   void   update(DirtySet& set, float delta);
   void   updateClient(float delta);
   void   draw() const;

 // operations
   Layer* createLayer();
   int    addLayer(Layer* player);
   int    layerIndex(const Layer& layer);
   void   moveLayer(Layer* player, int offset);

   Bound& addBound(const Vector& p1, const Vector& p2);
   void   removeBound(Bound& bound);

   void     addEntity(Entity* pentity);
   void     removeEntity(Id id);
   Entity*  findEntity(Id id);
   Entity&  resolveEntity(Id id);

 // notification
   void notifyObjectWorldCollision(Actor& object, Bound& bound, int side, bool begin);
   void notifyObjectObjectCollision(Actor& object, Actor& target, int side, bool begin);

 // rendering
   WorldRenderer* createRenderer();

protected:

   void           scroll();
   void           initializeBorders();
   
 // notifications
   void notifyLayerAdded(Layer& layer);
   void notifyScrollChange(const Vector& scrollposition);
   void notifyEntityAdded(const Entity& entity);
   void notifyEntityRemoved(const Entity& entity);

   const World&   me();

private:

   Layers      layers;
   Bounds      bounds;
   EntityMap   mEntities;
   Observers   mObservers;
   std::string mName;
   std::string mFilename;

   SimulationFactory*     mpSimulationFactory;
   Simulator*             mpSimulator;
   WorldSimulatorListener mSimulatorListener;

   Script* mpScript;

   LayerType _layerType;
   bool autoFollow;
   int followBorderWidth;
   int leftBorder, rightBorder, topBorder, bottomBorder;
   int _objectLayer;
   FollowMode followMode;
   Actor* followObject;
};

#ifdef JENGINE_INLINE
#  include "world.inl"
#endif

#endif
