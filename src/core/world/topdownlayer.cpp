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

#include "topdownlayer.h"

#include <algorithm>

#include "core/math/point.h"
#include "core/math/xform.h"
#include "core/math/matrix4.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/texture.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/viewport.h"

#include "layertype.h"
#include "tileset.h"

TopDownLayer::TopDownLayer():
   Layer()
{
}

TopDownLayer::~TopDownLayer()
{
}

LayerType TopDownLayer::getType() const
{
   return ETopDown;
}

bool TopDownLayer::initialize(Graphics::Device& device)
{
   if ( !Layer::initialize(device) )
   {
      return false;
   }

	return true;
}

void TopDownLayer::onViewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport)
{
	// calculate the maximum number of tiles on the screen
	maxTilesX = std::min(viewport.getWidth()  / mpTileSet->getTileWidth() + 1, getWidth());
   maxTilesY = std::min(viewport.getHeight() / mpTileSet->getTileHeight() + 1, getHeight());

   Layer::onViewportChanged(context, viewport);
}

/// \fn Layer::draw()
/// \brief Draws this layer on the screen.
/// \return Nothing
void TopDownLayer::draw(Graphics::RenderContext& context)
{
   if ( dirty )
   {
      updateBuffers(context);

      dirty = false;
	}

   if ( verts_to_render > 0 )
   {
      getEffect().enable(context);
      getEffect().setConstantBuffer(context, 0, *ub);

      context.getSpriteAtlas().bind(context, 0);

      context.setVertexBuffer(*vb);
      context.setIndexBuffer(*ib);
      context.drawTriangles(0, verts_to_render);

      vb->disable(context);
      ib->disable(context);
   }
}

void TopDownLayer::drawFront(Graphics::RenderContext& context)
{
   if ( verts_to_render_front > 0 )
   {
      getEffect().enable(context);
      getEffect().setConstantBuffer(context, 0, *ub);

      context.setVertexBuffer(*pfrontvb);
      context.setIndexBuffer(*ib);
      context.drawTriangles(0, verts_to_render_front);

      vb->disable(context);
      ib->disable(context);
   }
}

void TopDownLayer::updateTile(pv** pdata, int& indices, LayerLevel level, int x, int y, float xpos, float ypos)
{
   uint8_t texId = mTileMap.get(level, x, y);
	if ( texId < 255 )
   {
      TileInfo& info = (*mpTileSet)[texId];

      // see if the tile can be animated
      //if ( animateTiles && (info.flag & TileAnimate) )
      //      texId += info.anim_index;

      xpos += info.offset.x;
      ypos += info.offset.y;
            
      pv* pvertices = *pdata;
      
      if ( IS_SET(info.flag, TileRotated) )
      {
         pvertices[0].pos.set(xpos, ypos);
         pvertices[0].tex.set(info.coords.right, info.coords.top);
         pvertices[1].pos.set(xpos + tileset().getTileWidth(), ypos);
         pvertices[1].tex.set(info.coords.right, info.coords.bottom);
         pvertices[2].pos.set(xpos + tileset().getTileWidth(), ypos + tileset().getTileHeight());
         pvertices[2].tex.set(info.coords.left, info.coords.bottom);
         pvertices[3].pos.set(xpos, ypos + tileset().getTileHeight());
         pvertices[3].tex.set(info.coords.left, info.coords.top);
      }
      else
      {
         pvertices[0].pos.set(xpos, ypos);
         pvertices[0].tex.set(info.coords.left, info.coords.top);
         pvertices[1].pos.set(xpos + tileset().getTileWidth(), ypos);
         pvertices[1].tex.set(info.coords.right, info.coords.top);
         pvertices[2].pos.set(xpos + tileset().getTileWidth(), ypos + tileset().getTileHeight());
         pvertices[2].tex.set(info.coords.right, info.coords.bottom);
         pvertices[3].pos.set(xpos, ypos + tileset().getTileHeight());
         pvertices[3].tex.set(info.coords.left, info.coords.bottom);
      }
      
      (*pdata) += 4;
		indices += 6;
   }
}

void TopDownLayer::updateBuffers(Graphics::RenderContext& context)
{
   // determine the tile we are in
	int xstart = xscroll / mTileWidth;
	int ystart = yscroll / mTileHeight;

	int xend = xstart + maxTilesX + 1;
	int yend = ystart + maxTilesY + 1;

	// see if we have to render an extra tile
	if (((int)xscroll % mTileWidth) > (mTileWidth>>1)) xend++;
	if (((int)yscroll % mTileHeight) > (mTileHeight>>1)) yend++;

	// check on bounds
	if (xend > getWidth()) xend = getWidth();
	if (yend > getHeight()) yend = getHeight();

	float ypos = ystart * mTileHeight;

	verts_to_render = 0;
   verts_to_render_front = 0;
	pv* data = (pv*) vb->lock(context);
   pv* pfrontdata = (pv*) pfrontvb->lock(context);

  	for ( int y = ystart; y < yend; y++ )
   {
		float xpos = xstart * mTileWidth;

		for ( int x = xstart; x < xend; x++ )
      {
         updateTile(&data, verts_to_render, LayerLevel::eBack, x, y, xpos, ypos);
         updateTile(&data, verts_to_render, LayerLevel::eMid, x, y, xpos, ypos);
         updateTile(&pfrontdata, verts_to_render_front, LayerLevel::eFront, x, y, xpos, ypos);
         
			xpos += mTileWidth;
		}

      // (3D only: instead of + do a -)
		ypos += mTileHeight;
	}

	vb->unlock(context);
   pfrontvb->unlock(context);
}

c2d::Point TopDownLayer::pointToTile(const c2d::Point& point)
{
   if ( point.x < 0 || point.y < 0)
      return c2d::Point();

   int tileX = (point.x + xscroll) / mTileWidth;
   int tileY = (point.y + yscroll) / mTileHeight;

   return c2d::Point(tileX, tileY);
}
