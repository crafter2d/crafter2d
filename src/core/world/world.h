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

#include <vector>
#include <map>

#include "core/core_base.h"
#include "core/content/content.h"
#include "core/entity/idmanager.h"
#include "core/math/vector.h"
#include "core/string/string.h"

#include "bounds.h"
#include "layertype.h"
#include "worldobserver.h"
#include "worldsimulatorlistener.h"

namespace Graphics
{
   class Device;
   class RenderContext;
   class Viewport;
}

namespace c2d
{
   class ScriptObject;
}

class DirtySet;
class Entity;
class Layer;
class Bound;
class NodeVisitor;
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
class CORE_API World : public IContent
{
public:
   enum FollowMode { FollowObject=0, FollowMouse=1, NoFollow=2 };

   typedef std::vector<Layer*> Layers;
   typedef std::vector<WorldObserver*> Observers;
   typedef std::map<Id, Entity*> EntityMap;
   
   World();
   ~World();

   void  initialize(Graphics::Device& device);
   void  destroy();

 // get/set
   void           setAutoFollow(bool enabled=true);
   Entity*        getFollowObject();
   void           setFollowObject(Entity& obj);
   void           setScrollMode(int fm);
   void           setFollowBorderWidth(int width);
   void           setFollowBorders(int left, int right, int top, int bottom);
   void           setObjectLayer(int layer);

   const String&   getName() const;
   void            setName(const String& name);

   const Bound&   getBound(int index) const;
         Bound&   getBound(int index);
         int      getBoundCount() const;

   const Bounds&  getBounds() const;
         Bounds&  getBounds();

   int            getLayerCount() const;
   Layer*         getLayer(int index);
   const Layer*   getLayer(int index) const;
   int            getObjectLayer() const;
   
   LayerType      getLayerType() const;
   void           setLayerType(int type);

   Simulator&     getSimulator();
   void           setSimulator(Simulator* psimulator);

   bool               hasScript() const;
   c2d::ScriptObject& getScript();
   void               setScript(c2d::ScriptObject* pscript);

 // observer interface
   void  attach(WorldObserver& observer);
   void  detach(WorldObserver& observer);

 // visitor interface
   void   accept(NodeVisitor& nv);

 // update & drawing
   void   update(DirtySet& set, float delta);
   void   updateClient(Graphics::RenderContext& context, float delta);
   void   draw(Graphics::RenderContext& context) const;

 // operations
   Layer* createLayer();
   int    addLayer(Layer* player);
   void   removeLayer(Layer& layer);
   void   moveLayer(Layer* player, int offset);

   Bound& addBound(const Vector& p1, const Vector& p2);
   void   removeBound(Bound& bound);

   void     addEntity(Entity* pentity);
   void     removeEntity(Id id);
   Entity*  findEntity(Id id);
   Entity&  resolveEntity(Id id);

 // notification
   void notifyObjectWorldCollision(Entity& object, Bound& bound, int side, bool begin);
   void notifyObjectObjectCollision(Entity& object, Entity& target, int side, bool begin);

   void onViewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport);

 // rendering
   WorldRenderer* createRenderer();

protected:

   void           scroll(Graphics::RenderContext& context);
   void           initializeBorders(const Graphics::Viewport & viewport);
   
 // notifications
   void notifyScrollChange(const Vector& scrollposition);
   void notifyEntityAdded(const Entity& entity);
   void notifyEntityRemoved(const Entity& entity);

   const World&   me();

private:

 // operations
   void calculateScrollSpeed();

 // notifications
   void objectLayerChanged();

 // data
   String      mName;
   Layers      layers;
   Bounds      bounds;
   EntityMap   mEntities;
   Observers   mObservers;
   
   Simulator*             mpSimulator;          // owns
   WorldSimulatorListener mSimulatorListener;

   c2d::ScriptObject* mpScript;

   LayerType _layerType;
   bool autoFollow;
   int followBorderWidth;
   int leftBorder, rightBorder, topBorder, bottomBorder;
   int _objectLayer;
   FollowMode followMode;
   Entity* followObject;

   bool mBorderSet;
};

#ifdef JENGINE_INLINE
#  include "world.inl"
#endif

#endif
