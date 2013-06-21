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

#include "core/math/point.h"
#include "core/math/xform.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/texture.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/viewport.h"

#include "layertype.h"
#include "tile.h"
#include "tilerow.h"
#include "topdowntilerow.h"

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

   const TexturePtr diffuse = getEffect().resolveTexture("diffuseMap");
   float dx = diffuse->getSourceWidth() / diffuse->getWidth();
   float dy = diffuse->getSourceHeight() / diffuse->getHeight();

   texTileWidth  = dx * tileWidth;
   texTileHeight = dy * tileHeight;

   static const int border = 0;
   float borderx = dx * (tileWidth + border*2);
   float bordery = dy * (tileHeight + border*2);

   float offsetx = (borderx - texTileWidth) / 2;
   float offsety = (bordery - texTileHeight) / 2;

   // get number of tiles on one row in the diffuseMap
	maxTilesOnRow = diffuse->getWidth() / (tileWidth + border*2);

   // build the texture coord lookup table
   delete[] texcoordLookup;
	texcoordLookup = new Vector[tileCount+1];
	for (int tc = 0; tc < tileCount; tc++)
   {
		// calculate starting texture coordinates
		texcoordLookup[tc+1].x = offsetx + static_cast<float>(tc % maxTilesOnRow) * borderx + 0.0005f;
		texcoordLookup[tc+1].y = offsety + floorf ((float)tc / maxTilesOnRow) * bordery;
	}

   texTileWidth -= 0.001f;
   texTileHeight -= 0.001f;

	return true;
}

void TopDownLayer::onViewportChanged(const Graphics::Viewport& viewport)
{
	// calculate the maximum number of tiles on the screen
	maxTilesX = MIN(viewport.getWidth()  / mTileSet.getTileWidth() , getWidth());
   maxTilesY = MIN(viewport.getHeight() / mTileSet.getTileHeight(), getHeight());

	// calculate maximum tiles to scroll
	xscrollMax = MAX((getWidth() - maxTilesX) * tileWidth , 0);
	yscrollMax = MAX((getHeight() - maxTilesY) * tileHeight, 0);
}

/// \fn Layer::draw()
/// \brief Draws this layer on the screen.
/// \return Nothing
void TopDownLayer::draw(Graphics::RenderContext& context)
{
   //glPushMatrix ();
	//glTranslatef (-xscroll, -yscroll, 0);

	if ( dirty )
   {
		// determine the tile we are in
		int xstart = xscroll / tileWidth;
		int ystart = yscroll / tileHeight;

		int xend = xstart + maxTilesX + 1;
		int yend = ystart + maxTilesY + 1;

		// see if we have to render an extra tile
		if (((int)xscroll % tileWidth) > (tileWidth>>1)) xend++;
		if (((int)yscroll % tileHeight) > (tileHeight>>1)) yend++;

		// check on bounds
		if (xend > getWidth()) xend = getWidth();
		if (yend > getHeight()) yend = getHeight();

		float ypos = ystart * tileHeight;// - tileHeight;

		verts_to_render = 0;
		float* data = vb->lock (0);
      int index = 0;
      unsigned short* indexdata = (unsigned short*)ib->lock();

  	   for ( int y = ystart; y < yend; y++ )
      {
			float xpos = xstart * tileWidth;

			for ( int x = xstart; x < xend; x++ )
         {
				int texId = field[y][x].getTextureId();
				if (texId >= 1)
            {
               TileInfo& info = mTileSet[texId];

               // see if the tile can be animated
               if (animateTiles && info.flag & TileAnimate)
                   texId += info.anim_index;

					// calculate the position of the vertex
					float texX = texcoordLookup[texId].x;
					float texY = texcoordLookup[texId].y;

					// insert the vertices
					setVertex (&data, xpos                          , ypos                          , texX, texY);
               setVertex (&data, xpos+tileset().getTileWidth() , ypos                          , texX+texTileWidth, texY);
               setVertex (&data, xpos+tileset().getTileHeight(), ypos+tileset().getTileHeight(), texX+texTileWidth, texY+texTileHeight);
               setVertex (&data, xpos                          , ypos+tileset().getTileHeight(), texX, texY+texTileHeight);

               indexdata[0] = index + 0;
               indexdata[1] = index + 1;
               indexdata[2] = index + 2;
               indexdata[3] = index + 0;
               indexdata[4] = index + 2;
               indexdata[5] = index + 3;
               indexdata += 6;
               index += 4;

               // keep track of number of vertices
					verts_to_render += 6;
				}
				xpos += tileWidth;
			}
         // (3D only: instead of + do a -)
			ypos += tileHeight;
		}

		vb->unlock();
      ib->unlock();

      // reset the dirty flag
      dirty = false;
	}

   XForm matrix;
   matrix.setPosition(Vector(-xscroll, -yscroll));

   // draw layer at onces
   context.setWorldMatrix(matrix);
   context.setEffect(getEffect());
   context.setVertexBuffer(*vb);
   context.setIndexBuffer(*ib);

	context.drawTriangles(0, verts_to_render);
}

void TopDownLayer::drawHighlight(const Vector& point)
{
   float x = (point.x * tileWidth) - xscroll;
   float y = (point.y * tileHeight) - yscroll;

   /*
   glLineWidth(2.0f);

   glBegin(GL_LINE_LOOP);
   glVertex2f(x, y);
   glVertex2f(x, y + tileHeight);
   glVertex2f(x + tileWidth, y + tileHeight );
   glVertex2f(x + tileWidth, y);
   glEnd();

   glLineWidth(1.0f);
   */
}

TileRow* TopDownLayer::createTileRows(int width, int height)
{
   TileRow* prows = new TopDownTileRow[height];
   for ( int index = 0; index < height; ++index )
   {
      prows[index].create(width);
   }

   return prows;
}

Point TopDownLayer::pointToTile(const Point& point)
{
   if ( point.x() < 0 || point.y() < 0)
      return Point();

   int tileX = (point.x() + xscroll) / tileWidth;
   int tileY = (point.y() + yscroll) / tileHeight;

   return Point(tileX, tileY);
}
