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

#include <SDL/SDL.h>
#include <tinyxml.h>
#include <algorithm>
#include <functional>

#include "../net/bitstream.h"

#include "../log.h"
#include "../game.h"
#include "../gameconfiguration.h"
#include "../scenegraph.h"
#include "../console.h"
#include "../creature.h"
#include "../nodevisitor.h"

#include "layer.h"
#include "bound.h"
#include "layerfactory.h"
#include "worldreader.h"
#include "worldwriter.h"

static const std::string WORLD_EXTENSION = ".jwl";

IMPLEMENT_REPLICATABLE(WorldId, World, SceneObject)

/// fn World::World
/// brief Currently does nothing
World::World():
   layers(),
   bounds(),
   _observers(),
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
   if ( filename == NULL && xmlfile.empty() )
      return false;

   if ( filename != NULL )
      xmlfile = filename;

   std::string path = getFilename();

   WorldReader reader;
   if ( !reader.read(*this, path) )
      return false;

   initializeBorders();
   return true;
}

/// \fn World::createEmpty(const std::string& name)
/// \brief Initializes & saves this world as new file
void World::createEmpty(const std::string& name)
{
   xmlfile = name;
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
      AnimObject* obj = new AnimObject();
      obj->create(path.c_str());
      obj->setPosition(Vector(x,y));
      obj->setRotation(rotation);
      obj->setName(name.c_str());
      obj->setAnimation(1);

      // insert the object in the scenegraph
      add(obj, name.c_str());

      object = (TiXmlElement*)doc.IterateChildren("instance", object);
   }
}

/// \fn Worl::save()
/// \brief Writes this world to its file
bool World::save()
{
   std::string path = getFilename();

   WorldWriter writer;
   return writer.write(*this, path);
}

/// \fn World::destroy()
/// \brief Destroys the world object
void World::destroy()
{
   SceneObject::destroy();
   followObject = 0;

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

/// \fn World::getFilename() const
/// \brief Constructs complete filename including path and extension
std::string World::getFilename() const
{
   const std::string& path = Game::getInstance().getConfiguration().getWorldPath();
   return path + xmlfile + WORLD_EXTENSION;
}

/// \fn World::setObjectLayer(int layer)
/// \brief Set the layer on which the objects live.
void World::setObjectLayer(int objectlayerid)
{
   if ( objectlayerid < layers.size() )
   {
      Game& game = Game::getInstance();
      int screenWidth = game.getScreenWidth();
      int screenHeight = game.getScreenHeight();

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
      Console::getInstance().printf("World.setObjectLayer: trying to set invalid object layer (%d)", objectlayerid);
   }
}

/// \fn World::draw()
/// \brief Draws the world on screen
void World::draw ()
{
   // scroll if necessary
   if (autoFollow && followMode != NoFollow)
   {
      scroll ();
   }

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

/// \fn World::collide(Object& object)
/// \brief Performs collision detection with the object against the bounds
/// \param object Object to test for collisions with the bounds
int World::collide (Object& object) const
{
	Vector p3, p4, ip, vel;
	int collided = 0;

	float radius = object.getRadius ();
	vel = object.getVelocity ();

	p3 = object.getPosition ();
	p4.set (p3.x+vel.x, p3.y);

	Vector extra(vel.x,0);
	extra.normalize ();
	extra *= radius;
	p4 += extra;

	// perform collision detection for side movement
   for ( Bounds::size_type i = 0; i < bounds.size(); ++i )
   {
      Bound& bound = *bounds[i];
		Vector normal = bound.getNormal ();
		if (normal.x != 0)
      {
			if ( bound.intersect (p3, p4, ip) )
         {
				Vector normal = bound.getNormal ();
				if (normal.x != 0)
					vel.x = 0;

				normal *= radius;
				ip += normal;
				object.setPosition (ip);
				object.setVelocity (vel);
				p3 = ip;
				collided = 1;
			}
		}
	}

	p4.set (p3.x, p3.y + vel.y);
	extra.set (0, vel.y);
	extra.normalize ();
	extra *= radius;
	p4 += extra;

	// now perform collision detection for grafity
	for (Bounds::size_type i = 0; i < bounds.size(); ++i)
   {
      Bound& bound = *bounds[i];
		if ( bound.intersect (p3, p4, ip) )
      {
			Vector normal = bound.getNormal ();
			ip += (normal * radius);
			vel.y = 0;

         p3.y = ip.y;
			object.setPosition(p3);
			object.setVelocity(vel);

			//p3 = ip;
			collided = 2;
			break;
		}
	}

	return collided;
}

/// \fn World::scroll()
/// \brief Scrolls the layers based on the follow mode given by the application
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
      SDL_GetMouseState(&x, &y);
      const GuiRect& rect = Game::getInstance().getCanvas().getWindowRect();

      // get the window bounds
      if (x < followBorderWidth)
         xScroll = -1;
      else if (x > rect.getWidth() - followBorderWidth)
         xScroll = 1;

      if (y < followBorderWidth)
         yScroll = -1;
      else if (y > rect.getHeight() - followBorderWidth)
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
   const GuiRect& rect = Game::getInstance().getCanvas().getWindowRect();
   leftBorder = rect.left() + 50;
   rightBorder = rect.right() - 50;
   topBorder = rect.top() + 50;
   bottomBorder = rect.bottom() - 50;
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
