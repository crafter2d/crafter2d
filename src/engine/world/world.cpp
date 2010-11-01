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

#include <tinyxml.h>
#include <algorithm>
#include <functional>
#include <Box2D.h>

#include "core/log/log.h"
#include "core/defines.h"

#include "engine/physics/simulationfactory.h"
#include "engine/physics/simulator.h"

#include "engine/net/bitstream.h"

#include "engine/scenegraph.h"
#include "engine/creature.h"
#include "engine/nodevisitor.h"

#include "layer.h"
#include "layertype.h"
#include "bound.h"
#include "layerfactory.h"
#include "worldreader.h"
#include "worldwriter.h"
#include "topdownworldrenderer.h"

static const std::string WORLD_EXTENSION = ".jwl";

IMPLEMENT_REPLICATABLE(WorldId, World, SceneObject)

// static
bool World::isWorld(NetObject& object)
{
   return object.getRuntimeInfo().getName() == "World";
}

/// fn World::World
/// brief Currently does nothing
World::World():
   SceneObject(),
   layers(),
   bounds(),
   _observers(),
   mpSimulationFactory(NULL),
   mpSimulator(NULL),
   mSimulatorListener(*this),
   _layerType(ETopDown),
   autoFollow(true),
   followBorderWidth(5),
   leftBorder(0),
   rightBorder(0),
   topBorder(0),
   bottomBorder(0),
   _objectLayer(0),
   followMode(FollowMouse),
   followObject(0)

{
}

/*!
    \fn World::~World
	 \brief Currently does nothing
 */
World::~World()
{
   destroy();
}

/// \fn World::create(const char* filename)
/// \brief Loads world information from a file and preprocesses this information for use during the game
bool World::create (const char* filename)
{
   if ( filename == NULL && !hasFilename() )
      return false;

   if ( filename != NULL )
      setFilename(filename);

   std::string path = getFilename();

   WorldReader reader;
   if ( !reader.read(*this, path) )
      return false;

   initializeBorders();

   mpSimulator->setWorld(*this);
   mpSimulator->setListener(mSimulatorListener);

   return true;
}

/// \fn World::createEmpty(const std::string& name)
/// \brief Initializes & saves this world as new file
void World::createEmpty(const std::string& name)
{
   setFilename(name);
   save();

   initializeBorders();
}

void World::loadObjects(const char* filename)
{
   TiXmlElement* xmlElement;
   TiXmlText* xmlText;
   int rotation, x, y;

   // open the XML file
   TiXmlDocument doc(filename);
   if (!doc.LoadFile())
      return;

   TiXmlElement* object = (TiXmlElement*)doc.FirstChild("instance");
   while (object)
   {
      // load the blueprint
      xmlElement = (TiXmlElement*)object->FirstChild("blueprint");
      xmlText = (TiXmlText*)xmlElement->FirstChild();
      std::string blueprint = xmlText->Value();
      std::string path = blueprint + ".xml";

      // get the name of this object
      xmlElement = (TiXmlElement*)object->FirstChild("name");
      xmlText = (TiXmlText*)xmlElement->FirstChild();
      std::string name = xmlText->Value();

      // query the attributes of the object
      object->QueryIntAttribute("x", (int*)&x);
      object->QueryIntAttribute("y", (int*)&y);
      object->QueryIntAttribute("rot", &rotation);

      // create the new object
      //Creature* obj = new Creature();
      Object* obj = new Object();
      obj->create(path.c_str());
      obj->setPosition(Vector(x,y));
      obj->setRotation(rotation);
      obj->setName(name.c_str());
      obj->setAnimation(1);

      // insert the object in the scenegraph
      add(obj);

      object = (TiXmlElement*)doc.IterateChildren("instance", object);
   }
}

/// \fn Worl::save()
/// \brief Writes this world to its file
bool World::save()
{
   WorldWriter writer;
   return writer.write(*this, getFilename());
}

/// \fn World::destroy()
/// \brief Destroys the world object
void World::destroy()
{
   SceneObject::destroy();

   followObject = 0;

   delete mpSimulator;
   mpSimulator = NULL;

   if ( layers.size() > 0 )
   {
      for ( Layers::size_type idx = 0; idx < layers.size(); ++idx )
         delete layers[idx];

      layers.clear();
   }

   if ( bounds.size() > 0 )
   {
      for ( Bounds::size_type idx = 0; idx < bounds.size(); ++idx )
         delete bounds[idx];

      bounds.clear();
   }
}

// - Get/set

/// \fn World::setObjectLayer(int layer)
/// \brief Set the layer on which the objects live.
void World::setObjectLayer(int objectlayerid)
{
   if ( objectlayerid < layers.size() )
   {
      int screenWidth = 800;
      int screenHeight = 600;

      _objectLayer = objectlayerid;
      Layer* pobjectlayer = layers[objectlayerid];
      Vector area = pobjectlayer->getScrollArea();

      for ( int index = 0; index < objectlayerid; index++ )
      {
         if ( index != objectlayerid )
         {
            Layer* player = layers[index];
            player->calculateScrollSpeed(area, screenWidth, screenHeight);
         }
      }
   }
   else
   {
      Log::getInstance().warning("World.setObjectLayer: trying to set invalid object layer (%d)", objectlayerid);
   }
}

void World::setSimulationFactory(SimulationFactory& factory)
{
   mpSimulationFactory = &factory;
   mpSimulator = factory.createSimulator();
}

// - Operations

void World::doUpdate(float delta)
{
   getSimulator().run(delta);
}

void World::doUpdateClient(float delta)
{
   // scroll if necessary
   if (autoFollow && followMode != NoFollow)
   {
      scroll();
   }

   for ( int i = 0; i < layers.size(); i++ )
   {
      Layer& layer = *layers[i];
      layer.update(delta);
   }
}

const Vector& World::getPosition() const
{
   return Vector::zero();
}

/// \fn World::doDraw()
/// \brief Draws the world on screen
void World::doDraw ()
{
   // render the layers
   for ( int i = 0; i < layers.size(); i++ )
   {
      // enable the effect
      const Effect& effect = layers[i]->getEffect();
      effect.enable();

      // now render the layer
      layers[i]->draw();

      // quit the effect
      effect.disable();
   }

   // scroll the viewpoint to the right position
   Vector scroll = layers[getObjectLayer()]->getScroll ();
   glTranslatef(-scroll.x, -scroll.y, 0);
}

/// \fn World::scroll()
/// \brief Scrolls the layers based on the follow mode given by the application

// Move to the viewport!
void World::scroll ()
{
   float xScroll = 0;
   float yScroll = 0;

   if ( followMode == FollowObject )
   {
      if ( followObject != NULL )
      {
         Vector pos = followObject->getPosition ();
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
      int x, y;
      int width =800, height=600;
      // SDL_GetMouseState(&x, &y); <-- get it from the input of the client
      
      // get the window bounds
      if (x < followBorderWidth)
         xScroll = -1;
      else if (x > width - followBorderWidth)
         xScroll = 1;

      if (y < followBorderWidth)
         yScroll = -1;
      else if (y > height - followBorderWidth)
         yScroll = 1;
   }

   // scroll the layer
   if ( xScroll != 0 || yScroll != 0 )
   {
      Layer& layer = *layers[getObjectLayer()];
      Vector prescroll = layer.getScroll();

      for ( Layers::size_type l = 0; l < layers.size(); ++l )
         layers[l]->scroll(xScroll, yScroll);

      Vector scroll = layer.getScroll();

      if ( scroll != prescroll )
         notifyScrollChange(scroll);
   }
}

void World::initializeBorders()
{
   // set the follow object borders default values
   int width = 800, height = 600;
   
   leftBorder = 50;
   rightBorder = width - 50;
   topBorder = 50;
   bottomBorder = height - 50;
}

Layer* World::createLayer()
{
   return LayerFactory::getInstance().create(_layerType);
}

int World::addLayer(Layer* player)
{
   layers.push_back(player);

   notifyLayerAdded(*player);

   return layers.size() - 1;
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
         return NULL;

      case EInvalidLayerType:
      default:
         return NULL;
   }
}

//////////////////////////////////////////////////////////////////////////
// - (De)registration
//////////////////////////////////////////////////////////////////////////

void World::attach(WorldObserver& observer)
{
   _observers.push_back(&observer);
}

void World::detach(WorldObserver& observer)
{
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

class WorldLayerAddedNotify : public std::unary_function<WorldObserver*, void>
{
public:
   explicit WorldLayerAddedNotify(Layer& layer): _layer(layer) {}

   void operator()(WorldObserver* pobserver) const
   {
      pobserver->notifyLayerAdded(_layer);
   }

private:
   Layer& _layer;
};

void World::notifyLayerAdded(Layer& layer)
{
   std::for_each(_observers.begin(), _observers.end(), WorldLayerAddedNotify(layer));
}

class WorldScrollNotify : public std::unary_function<WorldObserver*, void>
{
public:
   explicit WorldScrollNotify(const Vector& scrollposition): _scrollposition(scrollposition) {}

   void operator()(WorldObserver* pobserver) const
   {
      pobserver->notifyScrollChange(_scrollposition);
   }

private:
   Vector _scrollposition;
};

void World::notifyScrollChange(const Vector& scrollposition)
{
   std::for_each(_observers.begin(), _observers.end(), WorldScrollNotify(scrollposition));
}

//////////////////////////////////////////////////////////////////////////
// - Serialization interface
//////////////////////////////////////////////////////////////////////////

void World::pack(BitStream& stream) const
{
   SceneObject::pack(stream);
}

void World::unpack(BitStream& stream)
{
   SceneObject::unpack(stream);
}
