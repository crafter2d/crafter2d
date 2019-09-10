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
#include "world.h"
#ifndef JENGINE_INLINE
#  include "world.inl"
#endif

#include <algorithm>
#include <functional>

#include "core/entity/entity.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/viewport.h"
#include "core/log/log.h"
#include "core/physics/simulator.h"
#include "core/script/scriptobject.h"

#include "bound.h"
#include "dirtyset.h"
#include "layer.h"
#include "layertype.h"
#include "layerfactory.h"
#include "nodevisitor.h"
#include "topdownworldrenderer.h"

using namespace Graphics;

World::World():
   mName(),
   layers(),
   bounds(),
   mEntities(),
   mObservers(),
   mpSimulator(nullptr),
   mSimulatorListener(*this),
   mpScript(nullptr),
   _layerType(ETopDown),
   autoFollow(true),
   followBorderWidth(5),
   leftBorder(0),
   rightBorder(0),
   topBorder(0),
   bottomBorder(0),
   _objectLayer(0),
   followMode(FollowMouse),
   followObject(nullptr),
   mBorderSet(false)
{
}

World::~World()
{
   destroy();
}

/// \fn World::initialize()
/// \brief Loads world information from a file and preprocesses this information for use during the game
void World::initialize(Device& device)
{
   for ( auto player : layers )
   {
      player->initialize(device);
   }
}

/// \fn World::destroy()
/// \brief Destroys the world object
void World::destroy()
{
   followObject = nullptr;

   delete mpSimulator;
   mpSimulator = nullptr;

   if ( layers.size() > 0 )
   {
      for ( auto player : layers )
         delete player;

      layers.clear();
   }

   if ( bounds.size() > 0 )
   {
      for ( auto pbound : bounds )
      {
         delete pbound;
      }

      bounds.clear();
   }

   if ( mEntities.size() > 0 )
   {
      for ( auto& it : mEntities )
      {
         Entity* pentity = it.second;
         delete pentity;
      }
   }

   delete mpScript;
   mpScript = nullptr;
}

// - Get/set

/// \fn World::setObjectLayer(int layer)
/// \brief Set the layer on which the objects live.
void World::setObjectLayer(int objectlayerid)
{
   if ( objectlayerid < layers.size() )
   {
      if ( objectlayerid != _objectLayer )
      {
         _objectLayer = objectlayerid;
         objectLayerChanged();
      }
   }
   else
   {
      Log::getInstance().warning("World.setObjectLayer: trying to set invalid object layer (%d)", objectlayerid);
   }
}

void World::setSimulator(Simulator* psimulator)
{
   mpSimulator = psimulator;
   mpSimulator->setWorld(*this);
   mpSimulator->setListener(mSimulatorListener);
}

// - Operations

void World::update(DirtySet& set, float delta)
{
   for ( auto& it : mEntities )
   {
      it.second->update(delta);
   }

   getSimulator().run(delta);

   for ( auto& it : mEntities )
   {
      Entity* pentity = it.second;
      if ( pentity->isDirty() )
      {
         set.reportDirty(*pentity);
      }
   }
}

void World::updateClient(Graphics::RenderContext& context, float delta)
{
   getSimulator().run(delta);

   // scroll if necessary
   if (autoFollow && followMode != NoFollow)
   {
      scroll(context);
   }

   for ( auto player : layers )
   {
      player->update(delta);
   }

   for ( auto& it : mEntities )
   {
      it.second->update(delta);
   }
}

/// \fn World::draw(Graphics::RenderContext& context)
/// \brief Draws the world on screen
void World::draw(Graphics::RenderContext& context) const
{
   // render the layers
   for ( auto player : layers )
   {
      player->draw(context);
   }
   
   // scroll the viewpoint to the right position
   Vector scroll = layers[getObjectLayer()]->getScroll ();
   context.setSpriteOffset(scroll);

   context.beginDraw();
   for ( auto& it : mEntities )
   {
      it.second->draw(context);
   }
   context.endDraw();

   // render the front layers
   for ( auto player : layers )
   {
      player->drawFront(context);
   }
}

void World::objectLayerChanged()
{
   calculateScrollSpeed();
}

void World::calculateScrollSpeed()
{
   Layer* pobjectlayer = layers[_objectLayer];
   Vector area = pobjectlayer->getScrollArea();

   for ( auto player : layers )
   {
      if ( player != pobjectlayer )
      {
         // calculate the scroll speed based on the object layer area
         player->calculateScrollSpeed(area);
      }
   }
}

/// \fn World::scroll()
/// \brief Scrolls the layers based on the follow mode given by the application
// Move to the viewport!
void World::scroll (Graphics::RenderContext& context)
{
   float xScroll = 0;
   float yScroll = 0;

   if ( followMode == FollowObject )
   {
      if ( followObject != nullptr )
      {
         Vector pos = followObject->getPosition();
         Layer& layer = *layers[getObjectLayer()];

         // determine if the layer should be scrolled
	      Vector lp = layer.layerToScreen(pos);
         if (lp.x < leftBorder)
            xScroll = lp.x - leftBorder;
         else if (lp.x >= rightBorder)
            xScroll = lp.x - rightBorder;

         if (lp.y < topBorder)
            yScroll = lp.y - topBorder;
         else if (lp.y > bottomBorder)
            yScroll = lp.y - bottomBorder;
      }
   }
   else if ( followMode == FollowMouse )
   {
      int x = 0, y = 0;
      int width =800, height=600;
      // SDL_GetMouseState(&x, &y); // <-- get it from the input of the client

      // get the window bounds
      if (x < followBorderWidth)
         xScroll = -(followBorderWidth - x);
      else if (x > width - followBorderWidth)
         xScroll = followBorderWidth - (width-x);

      if (y < followBorderWidth)
         yScroll = -(followBorderWidth - y);
      else if (y > height - followBorderWidth)
         yScroll = followBorderWidth - (height-y);
   }

   // scroll the layer
   //if ( xScroll != 0 || yScroll != 0 )
   {
      //Layer& layer = *layers[getObjectLayer()];
      //Vector prescroll = layer.getScroll();

      for ( auto player : layers )
      {
         player->scroll(context, xScroll, yScroll);
      }

      //Vector scroll = layer.getScroll();

      //if ( scroll != prescroll )
      //{
      //   notifyScrollChange(scroll);
      //}
   }
}

void World::initializeBorders(const Graphics::Viewport& viewport)
{
   if ( !mBorderSet )
   {
      //static const int Margin = 150;
      const int horizMargin = viewport.getWidth() / 4;
      const int vertMargin = viewport.getHeight() / 4;

      // set the follow object borders default values
      leftBorder = horizMargin;
      rightBorder = viewport.getWidth() - horizMargin;
      topBorder = vertMargin;
      bottomBorder = viewport.getHeight() - vertMargin;
   }
}

void World::onViewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport)
{
   for ( auto player : layers )
   {
      player->onViewportChanged(context, viewport);
   }

   calculateScrollSpeed();

   mBorderSet = false;
   initializeBorders(viewport);
}


Layer* World::createLayer()
{
   return LayerFactory::getInstance().create(_layerType);
}

int World::addLayer(Layer* player)
{
   layers.push_back(player);

   return layers.size() - 1;
}

void World::removeLayer(Layer& layer)
{
   Layers::iterator it = std::find(layers.begin(), layers.end(), &layer);
   layers.erase(it);
}

void World::moveLayer(Layer* player, int offset)
{
   Layers::iterator it = std::find(layers.begin(), layers.end(), player);

   Layers::iterator insert;
   if ( offset == -1 )
   {
      if ( it == layers.begin() )
         return;

      insert = it - 1;
   }
   else if ( offset == 1 )
   {
      insert = it + 1;
      if ( insert == layers.end() )
         return;
   }

   layers.erase(it);
   layers.insert(insert, player);
}

Bound& World::addBound(const Vector& p1, const Vector& p2)
{
   Bound* pbound = new Bound(p1, p2);
   bounds.push_back(pbound);

   return *pbound;
}

void World::removeBound(Bound& bound)
{
   Bounds::iterator it = std::find(bounds.begin(), bounds.end(), &bound);
   if ( it != bounds.end() )
      bounds.erase(it);
}

WorldRenderer* World::createRenderer()
{
   switch ( _layerType )
   {
      case ETopDown:
         return new TopDownWorldRenderer(*this);
      case EIsoDiamond:
         return nullptr;

      case EInvalidLayerType:
      default:
         return nullptr;
   }
}

//-----------------------------------
// - Entities
//-----------------------------------

Entity* World::findEntity(Id id)
{
   EntityMap::iterator it = mEntities.find(id);
   return it != mEntities.end() ? it->second : nullptr;
}

Entity& World::resolveEntity(Id id)
{
   Entity* pentity = findEntity(id);
   ASSERT_PTR(pentity);
   return *pentity;
}

void World::addEntity(Entity* pentity)
{
   mEntities[pentity->getId()] = pentity;

   notifyEntityAdded(*pentity);

   if ( mpScript )
   {
      mpScript->prepareCall(1);
      mpScript->arg(0, pentity->getClassName(), pentity);
      mpScript->call(UTEXT("onEntityAdded"));
   }
}

void World::removeEntity(Id id)
{
   EntityMap::iterator it = mEntities.find(id);
   if ( it != mEntities.end() )
   {
      notifyEntityRemoved(*(it->second));

      delete it->second;

      mEntities.erase(it);
   }
}

//////////////////////////////////////////////////////////////////////////
// - (De)registration
//////////////////////////////////////////////////////////////////////////

void World::attach(WorldObserver& observer)
{
   mObservers.push_back(&observer);
}

void World::detach(WorldObserver& observer)
{
   Observers::iterator it = std::find(mObservers.begin(), mObservers.end(), &observer);
   if ( it != mObservers.end() )
   {
      mObservers.erase(it);
   }
   // remove observer from the array
}

//////////////////////////////////////////////////////////////////////////
// - Visitor interface
//////////////////////////////////////////////////////////////////////////

void World::accept(NodeVisitor& nv)
{
   nv.visitWorld(this);
}

//////////////////////////////////////////////////////////////////////////
// - Notifications
//////////////////////////////////////////////////////////////////////////

void World::notifyScrollChange(const Vector& scrollposition)
{
   for ( WorldObserver* pobserver : mObservers )
   {
      pobserver->notifyScrollChange(scrollposition);
   }
}

void World::notifyEntityAdded(const Entity& entity)
{
   for ( WorldObserver* pobserver : mObservers )
   {
      pobserver->notifyEntityAdded(entity);
   }
}

void World::notifyEntityRemoved(const Entity& entity)
{
   for ( WorldObserver* pobserver : mObservers )
   {
      pobserver->notifyEntityRemoved(entity);
   }
}

// - Script functions

static const String sBound = UTEXT("engine.game.Bound");
static const String sCollision = UTEXT("onObjectCollision");

void World::notifyObjectWorldCollision(Entity& object, Bound& bound, int side, bool begin)
{
   ASSERT_PTR(mpScript);
   mpScript->prepareCall(4);
   mpScript->arg(0, object.getClassName(), &object);
   mpScript->arg(1, sBound, &bound);
   mpScript->arg(2, side);
   mpScript->arg(3, begin);
   mpScript->call(sCollision);
}

void World::notifyObjectObjectCollision(Entity& source, Entity& target, int side, bool begin)
{
   ASSERT_PTR(mpScript);
   mpScript->prepareCall(4);
   mpScript->arg(0, source.getClassName(), &source);
   mpScript->arg(1, target.getClassName(), &target);
   mpScript->arg(2, side);
   mpScript->arg(3, begin);
   mpScript->call(sCollision);
}
