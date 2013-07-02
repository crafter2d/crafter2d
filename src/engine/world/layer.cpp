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

#include "layer.h"
#ifndef JENGINE_INLINE
#  include "layer.inl"
#endif

#include <fstream>
#include <string>
#include <cstdlib>
#include <math.h>

#include "core/graphics/device.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/vertexbuffer.h"
#include "core/smartptr/autoptr.h"
#include "core/log/log.h"

#include "tile.h"
#include "tilerow.h"

using namespace Graphics;

/// \fn Layer::Layer()
/// \brief Initializes the member variables.
Layer::Layer():
   tileWidth(0),
   tileHeight(0),
   tileCount(0),
   texTileWidth(0),
   texTileHeight(0),
   maxTilesOnRow(0),
   xscroll(0),
   yscroll(0),
   xscrollMax(0),
   yscrollMax(0),
   scrollSpeedX(1),
   scrollSpeedY(1),
   maxTilesX(0),
   maxTilesY(0),
   verts(0),
   verts_to_render(0),
   animateTiles(true),
   dirty(true),
   mTileSet(),
   field(0),
   mInputLayout(Graphics::INPUT_XY | Graphics::INPUT_Tex0),
   vb(NULL),
   ib(NULL),
   texcoordLookup(0),
   mpDefinition(NULL),
   mEffect(mInputLayout)
{
}

/// \fn Layer::~Layer()
/// \brief Releases the member variables.
Layer::~Layer()
{
	release();
}

bool Layer::create(LayerDefinition* pdefinition)
{
   setDefinition(pdefinition);

   if (getWidth() <= 0 || getHeight() <= 0) 
   {
      Log::getInstance().error("Layer.create: Invalid layer size!");
		return false;
	}

   field = createTileRows(getWidth(), getHeight());

   return true;
}

/// \fn Layer::~Layer()
/// \brief Create a new empty layer
bool Layer::initialize(Device& device)
{
   if ( !mEffect.load(device, mpDefinition->effect) )
   {
      std::string file = mpDefinition->effect.toUtf8();
      Log::getInstance().error("Can not load effect file '%s'.", file.c_str());
      return false;
   }

   if ( !createBuffers(device, getWidth(), getHeight()) )
   {
      return false;
   }

   // load the tileset
   const TexturePtr diffuse = mEffect.resolveTexture(UTEXT("diffuseMap"));
   String tileInfo = diffuse.getName();
   int pos = tileInfo.lastIndexOf('.');
   tileInfo.replace(pos+1, 3, UTEXT("xml"));

   if ( !mTileSet.create(tileInfo) )
      return false;

   tileCount = tileset().getTileCount();
   tileWidth = tileset().getTileWidth();
   tileHeight = tileset().getTileHeight();

   return true;
}

/// \fn Layer::onViewportChanged(const Graphics::Viewport& viewport)
/// \brief Called when the viewports dimensions have been changed. Precalculates maximum allowable scrolling 
/// values and initializes the OpenGL object of the layer for rendering.
/// \retval true the layer was successfully prepaired
/// \retval false the vertex buffer could not be created
void Layer::onViewportChanged(const Graphics::Viewport& viewport)
{
   PURE_VIRTUAL;
}

Vector Layer::getScrollArea() const
{
   int tilesX = MAX(getWidth() - maxTilesX, 0);
   int tilesY = MAX(getHeight() - maxTilesY, 0);

   return Vector(tileset().getTileWidth() * tilesX, mTileSet.getTileHeight() * tilesY);
}


void Layer::calculateScrollSpeed(const Vector& area, int screenWidth, int screenHeight)
{
   Vector thisarea = getScrollArea();

   scrollSpeedX = thisarea.x / area.x;
   scrollSpeedY = thisarea.y / area.y;
}

/// \fn Layer::release()
/// \brief Releases the internal memory
/// \return Nothing
void Layer::release()
{
	mEffect.destroy ();

	delete[] texcoordLookup;
	delete[] field;
	delete vb;
}

void Layer::update(float delta)
{
   if ( animateTiles )
   {
      dirty |= mTileSet.update(delta);
   }
}

//////////////////////////////////////////////////////////////////////////
// - Drawing
//////////////////////////////////////////////////////////////////////////

/// \fn Layer::draw()
/// \brief Pure virtual draw method. Should be overloaded by sub-classes.
/// \return Nothing
void Layer::draw(Graphics::RenderContext& context)
{
   PURE_VIRTUAL
}

void Layer::drawHighlight(const Vector& point)
{
   PURE_VIRTUAL
}

//////////////////////////////////////////////////////////////////////////
// - Get/set interface
//////////////////////////////////////////////////////////////////////////

LayerType Layer::getType() const
{
   PURE_VIRTUAL
   return EInvalidLayerType;
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

TileRow* Layer::createTileRows(int width, int height)
{
   PURE_VIRTUAL
   return NULL;
}

bool Layer::createBuffers(Device& device, int width, int height)
{
   vb = device.createVertexBuffer(mInputLayout);
   if ( vb == NULL )
   {
      Log::getInstance().error("Not enough memory available for vertex buffer.");
      return false;
   }

   // create the vertex buffer for this layer
   int usage = VertexBuffer::eWriteOnly | VertexBuffer::eDynamic;
   int size = width * height * 4; // each tile is 4 vertices

   if ( !vb->create(size, usage) )
   {
		Log::getInstance().error("Could not create the vertex buffer.");
		return false;
	}

   ib = device.createIndexBuffer();
   if ( ib == NULL )
   {
      return false;
   }

   size = width * height * 6; // each tile is 6 indices
   if ( !ib->create(IndexBuffer::eShort, size, NULL) )
   {
      return false;
   }

   return true;
}

/// \fn Layer::scroll(float x, float y)
/// \brief Moves the layer around in the screen
/// \param[in] x amount layer should horizontally scroll; negative for left and positive for right
/// \param[in] y amount layer should vertically scroll; negative for up and positive for down
void Layer::scroll (float x, float y)
{
   if (x != 0 || y != 0)
   {
      float oldxscroll = xscroll;
	   xscroll += (x * scrollSpeedX);
	   if (xscroll < 0)
         xscroll = 0;
	   else if (xscroll > xscrollMax)
         xscroll = xscrollMax;

      float oldyscroll = yscroll;
	   yscroll += (y * scrollSpeedY);
	   if (yscroll < 0)
         yscroll = 0;
	   else if (yscroll > yscrollMax)
         yscroll = yscrollMax;

      if ( oldxscroll != xscroll || oldyscroll != yscroll )
         dirty = true;
   }
}

/// \fn Layer::getTile(int x, int y)
/// \brief Returns the tile the given indices; -1 if the position is outside the layer.
int Layer::getTile(int x, int y) const
{
   if ( x < 0 || y < 0 || x >= getWidth() || y >= getHeight() )
   {
      return -1;
   }

   return field[y][x].getTextureId();
}

/// \fn Layer:setTile(int x, int y, int tile)
/// \breif Set the new tile texture.
void Layer::setTile(int x, int y, int tile)
{
   if ( x >= 0 && y >= 0 && x < getWidth() && y < getHeight() )
   {
      field[y][x].setTextureId(tile);

      dirty = true;
   }
}
