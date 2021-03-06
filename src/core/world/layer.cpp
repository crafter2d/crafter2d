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
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/viewport.h"
#include "core/graphics/texture.h"
#include "core/graphics/utils.h"
#include "core/math/matrix4.h"
#include "core/log/log.h"
#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"

#include "tilefield.h"

using namespace Graphics;

/// \fn Layer::Layer()
/// \brief Initializes the member variables.
Layer::Layer():
   mTileWidth(0),
   mTileHeight(0),
   xscroll(0),
   yscroll(0),
   xscrollMax(0),
   yscrollMax(0),
   scrollSpeedX(1),
   scrollSpeedY(1),
   maxTilesX(0),
   maxTilesY(0),
   verts_to_render(0),
   verts_to_render_front(0),
   animateTiles(true),
   dirty(true),
   mpTileSet(nullptr),
   mTileMap(),
   vb(nullptr),
   pfrontvb(nullptr),
   ib(nullptr),
   ub(nullptr),
   mpDefinition(nullptr),
   mpEffect(nullptr),
   mConstants()
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
   
   return true;
}

/// \fn Layer::~Layer()
/// \brief Create a new empty layer
bool Layer::initialize(Device& device)
{
   mpEffect = device.createEffect(UTEXT("shaders/basic")); //mpDefinition->effect);
   if ( mpEffect == nullptr )
   {
      return false;
   }

   if ( !createBuffers(device, getWidth(), getHeight())
     || !createUniformBuffers(device) )
   {
      return false;
   }

   mTileWidth = tileset().getTileWidth();
   mTileHeight = tileset().getTileHeight();

   return true;
}

/// \fn Layer::release()
/// \brief Releases the internal memory
/// \return Nothing
void Layer::release()
{
   delete ub;
   delete ib;
   delete vb;
   delete pfrontvb;
   delete mpEffect;
}

#include <algorithm>

/// \fn Layer::onViewportChanged(const Graphics::RenderContext& context, const Graphics::Viewport& viewport)
/// \brief Called when the viewports dimensions have been changed. 
void Layer::onViewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport)
{
   // calculate maximum pixels to scroll
   xscrollMax = std::max(getWidth() * mTileWidth - viewport.getWidth(), 0);
   yscrollMax = std::max(getHeight() * mTileHeight - viewport.getHeight(), 0);

   // update scroll position to stay within the valid boundaries
   xscroll = std::max(0.0f, std::min(xscrollMax, xscroll));
   yscroll = std::max(0.0f, std::min(yscrollMax, yscroll));

   mConstants.projection.setOrtho(viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);
   mConstants.world.setTranslation(-xscroll, -yscroll, 0.0f);
   ub->set(context, &mConstants, sizeof(mConstants));

   dirty = true;
}

Vector Layer::getScrollArea() const
{
   return Vector(xscrollMax, yscrollMax);
}

void Layer::calculateScrollSpeed(const Vector& area)
{
   scrollSpeedX = 0.0f;
   scrollSpeedY = 0.0f;

   if ( xscrollMax > 0.0f && area.x > 0.0f )
   {
      scrollSpeedX = xscrollMax / area.x;
   }

   if ( yscrollMax > 0.0f && area.y > 0.0f )
   {
      scrollSpeedY = yscrollMax / area.y;
   }
}

void Layer::update(float delta)
{
   if ( animateTiles )
   {
      dirty |= mpTileSet->update(delta);
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
   C2D_UNUSED(context);
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

void Layer::resize(int width, int height)
{
   mTileMap.resize(width, height);

   mpDefinition->width = width;
   mpDefinition->height = height;
}

bool Layer::createBuffers(Device& device, int width, int height)
{
   ASSERT_PTR(mpEffect);

   const int batchsize = width * height;
   int usage = VertexBuffer::eWriteOnly | VertexBuffer::eDynamic;

   int size = 2 * batchsize * 4; // two layers, each tile is 4 vertices
   vb = mpEffect->createVertexBuffer(device, size, usage);
   if ( vb == nullptr )
   {
      Log::getInstance().error("Could not create the vertex buffer.");
      return false;
   }

   size = batchsize * 4; // one layers, each tile is 4 vertices
   pfrontvb = mpEffect->createVertexBuffer(device, size, usage);
   if ( pfrontvb == nullptr )
   {
      Log::getInstance().error("Could not create the vertex buffer.");
      return false;
   }

   ib = Utils::createIndexBuffer(device, batchsize, 4);
   if ( ib == nullptr )
   {
      return false;
   }
   
   return true;
}

bool Layer::createUniformBuffers(Graphics::Device& device)
{
   UNIFORM_BUFFER_DESC descs[] = {
      { UTEXT("proj"), sizeof(float) * 16 },
      { UTEXT("world"), sizeof(float) * 16 },
      { UTEXT("object"), sizeof(float) * 16 },
   };

   ub = getEffect().createUniformBuffer(UTEXT("mpv"));
   if ( ub == nullptr || !ub->create(device, descs, 3) )
   {
      // could not create the uniform buffer object
      // perhaps wrong name??
      return false;
   }

   return true;
}

/// \fn Layer::scroll(Graphics::RenderContext& context, float x, float y)
/// \brief Moves the layer around in the screen
/// \param[in] context the graphics context
/// \param[in] x amount layer should horizontally scroll; negative for left and positive for right
/// \param[in] y amount layer should vertically scroll; negative for up and positive for down
void Layer::scroll(Graphics::RenderContext& context, float x, float y)
{
   if (x != 0 || y != 0)
   {
      float oldxscroll = xscroll;
	   xscroll += (x * scrollSpeedX);
      xscroll = std::max(0.0f, std::min(xscrollMax, xscroll));

      float oldyscroll = yscroll;
	   yscroll += (y * scrollSpeedY);
      yscroll = std::max(0.0f, std::min(yscrollMax, yscroll));

      if ( oldxscroll != xscroll || oldyscroll != yscroll )
      {
         mConstants.world.setTranslation(-xscroll, -yscroll, 0);
         ub->set(context, &mConstants, sizeof(mConstants));

         dirty = true;
      }
   }
}

/// \fn Layer::getTile(int x, int y)
/// \brief Returns the tile the given indices; -1 if the position is outside the layer.
uint8_t Layer::getTile(LayerLevel level, int x, int y) const
{
   return mTileMap.get(level, x, y);
}

/// \fn Layer:setTile(int x, int y, uint8_t tile)
/// \breif Set the new tile texture.
void Layer::setTile(LayerLevel level, int x, int y, uint8_t tile)
{
   mTileMap.set(level, x, y, tile);
   
   dirty = true;
}
