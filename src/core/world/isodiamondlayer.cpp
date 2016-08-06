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

#include "isodiamondlayer.h"

#include "core/math/point.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/texture.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/viewport.h"

#include "layertype.h"

IsoDiamondLayer::IsoDiamondLayer():
   _halfTileWidth(0.0f),
   _halfTileHeight(0.0f),
   _xstart(0.0f)
{
}

IsoDiamondLayer::~IsoDiamondLayer()
{
}

bool IsoDiamondLayer::initialize(Graphics::Device& device)
{
   if ( !Layer::initialize(device) )
   {
      return false;
   }

   tileWidth = 40;
   tileHeight = 20;

   /*
   _halfTileWidth  = static_cast<float>(tileWidth) / 2;
   _halfTileHeight = static_cast<float>(tileHeight) / 2;

   // calculate start offset
   _xstart = tileWidth * (getWidth() / 2);// + _halfTileWidth;

   // get number of tiles on one row in the diffuseMap
   const Graphics::Texture& diffuse = mpTileSet->getTexture();
	maxTilesOnRow = diffuse.getWidth() / tileWidth;

   float nX = static_cast<float>(diffuse.getWidth()) / tileWidth;
   float nY = static_cast<float>(diffuse.getHeight()) / tileHeight;

   texTileWidth  = diffuse.getSourceWidth() / nX;
   texTileHeight = diffuse.getSourceHeight() / nY;

   tileCount = 10;

   // build the texture coord lookup table
	texcoordLookup = new Vector[tileCount+1];
	for (int tc = 0; tc < tileCount; tc++)
   {
		// calculate starting texture coordinates
		texcoordLookup[tc+1].x = static_cast<float>((tc % maxTilesOnRow) * texTileWidth);
		texcoordLookup[tc+1].y = floorf ((float)tc / maxTilesOnRow) * texTileHeight;
	}
   */
   return true;
}

LayerType IsoDiamondLayer::getType() const
{
   return EIsoDiamond;
}

float IsoDiamondLayer::getXOffset() const
{
   return _xstart;
}

void IsoDiamondLayer::onViewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport)
{
   Layer::onViewportChanged(context, viewport);
}

void IsoDiamondLayer::draw(Graphics::RenderContext& context)
{
   float* data = (float*) vb->lock(context);

   float xstart = _xstart - xscroll;
   float ystart = -yscroll;

   float xpos = xstart;
   float ypos = 0;

   int verts_to_render = 0;

   for ( int y = 0; y < getHeight(); ++y )
   {
      xpos = xstart;
      ypos = ystart;

      for ( int x = 0; x < getWidth(); ++x )
      {
         drawTile(&data, LayerLevel::eBack, x, y, xpos, ypos);
         drawTile(&data, LayerLevel::eMid, x, y, xpos, ypos);
         drawTile(&data, LayerLevel::eFront, x, y, xpos, ypos);

         xpos += _halfTileWidth;
         ypos += _halfTileHeight;
      }

      xstart -= _halfTileWidth;
      ystart += _halfTileHeight;
   }

   vb->unlock(context);

   // draw layer at onces
	vb->enable(context);
   context.drawTriangles(0, verts_to_render);
	vb->disable(context);
}

void IsoDiamondLayer::drawTile(float** pdata, LayerLevel level, int x, int y, int xpos, int ypos)
{
   int texId = mTileMap.get(level, x, y);
   if ( texId >= 0 )
   {
      /*TileInfo& info = (*mpTileSet)[texId];

      float halfwidth  = texTileWidth / 2;
      float halfheight = texTileHeight / 2;

      setVertex(pdata, xpos - _halfTileWidth, ypos,                     tx,               ty + halfheight);
      setVertex(pdata, xpos,                  ypos - _halfTileHeight,   tx + halfwidth,   ty);
      setVertex(pdata, xpos + _halfTileWidth, ypos,                     tx + texTileWidth,ty + halfheight);
      setVertex(pdata, xpos,                  ypos + _halfTileHeight,   tx + halfwidth,   ty + texTileHeight);

      verts_to_render += 4;*/
   }
}

void IsoDiamondLayer::drawFront(Graphics::RenderContext& context)
{
   C2D_UNUSED(context);
   // nothing here yet
}

c2d::Point IsoDiamondLayer::pointToTile(const c2d::Point& point)
{
   c2d::Point p(point.x - getXOffset() + xscroll, point.y + yscroll);

   int y = (2 * p.y - p.x) / 2;
   int x = p.x + y;

   float xtile = static_cast<float>(x) / tileheight() + 0.5f;
   float ytile = static_cast<float>(y) / tileheight() + 0.5f;

   return c2d::Point(xtile, ytile);
}
