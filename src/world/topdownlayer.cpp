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

#include "../opengl.h"
#include "../console.h"
#include "../vertexbuffer.h"

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

bool TopDownLayer::prepare(int screenWidth, int screenHeight)
{
   int border = 0;

   // get number of tiles on one row in the diffuseMap
	const Texture& diffuse = effect.resolveTexture("diffuseMap");
	maxTilesOnRow = diffuse.getWidth() / (tileWidth + border*2);

	// calculate the maximum number of tiles on the screen
	maxTilesX = MIN(screenWidth  / mTileSet.getTileWidth() , width);
	maxTilesY = MIN(screenHeight / mTileSet.getTileHeight(), height);

	// calculate maximum tiles to scroll
	xscrollMax = MAX((width  - maxTilesX) * tileWidth , 0);
	yscrollMax = MAX((height - maxTilesY) * tileHeight, 0);

   delete vb;
   vb = createVertexBuffer(width, height, 4);
   if ( vb == NULL )
      return false;

   float dx = diffuse.getSourceWidth() / diffuse.getWidth();
   float dy = diffuse.getSourceHeight() / diffuse.getHeight();

   texTileWidth  = dx * tileWidth;
   texTileHeight = dy * tileHeight;

   float borderx = dx * (tileWidth + border*2);
   float bordery = dy * (tileHeight + border*2);

   float offsetx = (borderx - texTileWidth) / 2;
   float offsety = (bordery - texTileHeight) / 2;

   // build the texture coord lookup table
   delete[] texcoordLookup;
	texcoordLookup = new Vector[tileCount+1];
	for (int tc = 0; tc < tileCount; tc++)
   {
		// calculate starting texture coordinates
		texcoordLookup[tc+1].x = offsetx + static_cast<float>(tc % maxTilesOnRow) * borderx;
		texcoordLookup[tc+1].y = offsety + floorf ((float)tc / maxTilesOnRow) * bordery;
	}

   texTileWidth -= 0.001;
   texTileHeight -= 0.001;

	return true;
}

/// \fn Layer::draw()
/// \brief Draws this layer on the screen.
/// \return Nothing
void TopDownLayer::draw ()
{
   glPushMatrix ();
	glTranslatef (-xscroll, -yscroll, 0);

   if (animateTiles )
   {
      // update the tileset
      dirty |= tileset().update (SDL_GetTicks());
   }

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
		if (xend > width) xend = width;
		if (yend > height) yend = height;

		float ypos = ystart * tileHeight;// - tileHeight;

		verts_to_render = 0;
		float* data = vb->lock (0);

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
					setVertex (&data, xpos                          , ypos                          , 0.0005+texX, texY);
               setVertex (&data, xpos+tileset().getTileWidth() , ypos                          , 0.0005+texX+texTileWidth, texY);
               setVertex (&data, xpos+tileset().getTileHeight(), ypos+tileset().getTileHeight(), 0.0005+texX+texTileWidth, texY+texTileHeight);
               setVertex (&data, xpos                          , ypos+tileset().getTileHeight(), 0.0005+texX, texY+texTileHeight);

					// keep track of number of vertices
					verts_to_render += 4;
				}
				xpos += tileWidth;
			}
         // (3D only: instead of + do a -)
			ypos += tileHeight;
		}

		vb->unlock ();

      // reset the dirty flag
      dirty = false;
	}

   // draw layer at onces
	vb->enable();
	glDrawArrays(GL_QUADS, 0, verts_to_render);
	vb->disable();

	glPopMatrix();
}

void TopDownLayer::drawHighlight(const Vector& point)
{
   float x = (point.x * tileWidth) - xscroll;
   float y = (point.y * tileHeight) - yscroll;

   glLineWidth(2.0f);

   glBegin(GL_LINE_LOOP);
   glVertex2f(x, y);
   glVertex2f(x, y + tileHeight);
   glVertex2f(x + tileWidth, y + tileHeight );
   glVertex2f(x + tileWidth, y);
   glEnd();

   glLineWidth(1.0f);
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
