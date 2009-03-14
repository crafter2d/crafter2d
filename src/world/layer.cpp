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

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <fstream>
#include <string>
#include <cstdlib>
#include <GL/GLee.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <math.h>

#include "../game.h"
#include "../gameconfiguration.h"
#include "../opengl.h"
#include "../console.h"
#include "../vertexbuffer.h"

#include "tile.h"
#include "tilerow.h"

/// \fn Layer::Layer()
/// \brief Initializes the member variables.
Layer::Layer():
   tileWidth(0),
   tileHeight(0),
   tileCount(0),
   maxTilesOnRow(0),
   width(0),
   height(0),
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
   name(),
   effectFile(),
   mTileSet(),
   field(0),
   vb(0),
   texcoordLookup(0)
{
}

/// \fn Layer::~Layer()
/// \brief Releases the member variables.
Layer::~Layer()
{
	release();
}

/// \fn Layer::~Layer()
/// \brief Create a new empty layer
bool Layer::create(const std::string& layername, int w, int h, const std::string& effectname)
{
   name        = layername;
   effectFile  = effectname;
   width       = w;
   height      = h;

   if (width <= 0 || height <= 0) {
      Console::getInstance().printf("Layer.create: Invalid layer size!");
		return false;
	}

   field = createTileRows(width, height);

   const std::string& effectpath = Game::getInstance().getConfiguration().getShaderPath();
   std::string effectPath = effectpath + effectFile + ".xml";
   if ( !effect.load(effectPath.c_str()) )
   {
      Console::getInstance().printf("Can not load effect file '%s'.", effectFile.c_str());
      return false;
   }

   // load the tileset
   const Texture& diffuse = effect.resolveTexture("diffuseMap");
   std::string tileInfo = diffuse.getFileName();
   std::size_t pos = tileInfo.rfind('.');
   tileInfo.replace(pos+1,3,"xml");

   if ( !mTileSet.create(tileInfo.c_str()) )
      return false;

   tileCount = tileset().getTileCount();
   tileWidth = tileset().getTileWidth();
   tileHeight = tileset().getTileHeight();

   return prepare();
}

/// \fn Layer::prepare()
/// \brief Precalculates values and initializes the OpenGL object of the layer for rendering.
/// \retval true the layer was successfully prepaired
/// \retval false the vertex buffer could not be created
bool Layer::prepare()
{
   GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

   // calculate maximum allowable scrolling, world is now moving in a negative manner
	// so negate the values for run-time improvement.
	int screenWidth = viewport[2] - viewport[0];
	int screenHeight = viewport[3] - viewport[1];

   return prepare(screenWidth, screenHeight);
}

bool Layer::prepare(int screenWidth, int screenHeight)
{
   PURE_VIRTUAL
   return false;
}

Vector Layer::getScrollArea() const
{
   int tilesX = MAX(width - maxTilesX, 0);
   int tilesY = MAX(height - maxTilesY, 0);

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
	effect.destroy ();

	delete[] texcoordLookup;
	delete[] field;
	delete vb;
}

//////////////////////////////////////////////////////////////////////////
// - Drawing
//////////////////////////////////////////////////////////////////////////

/// \fn Layer::draw()
/// \brief Pure virtual draw method. Should be overloaded by sub-classes.
/// \return Nothing
void Layer::draw()
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

VertexBuffer* Layer::createVertexBuffer(int width, int height, int vertexcount)
{
   // create the vertex buffer for this layer
	VertexBuffer* pvb = OpenGL::createVertexBuffer();
   if ( pvb == NULL )
   {
      Console::getInstance().print("Not enough memory available for vertex buffer.");
      return NULL;
   }

   if ( !pvb->create (effect, width*height*vertexcount, USAGE_DYNAMIC|USAGE_WRITEONLY, FVF_XY|FVF_TEX0) )
   {
		Console::getInstance().print("Could not create the vertex buffer.");

      delete pvb;
		return NULL;
	}

   return pvb;
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
/// \brief Returns the tile the user for a position; -1 if the position is outside the layer.
int Layer::getTile(int x, int y) const
{
   if (x < 0 || y < 0)
      return -1;

   int tileX = x / tileWidth;
   int tileY = y / tileHeight;
   if (tileX >= width || tileY >= height)
      return -1;
   else
      return field[tileY][tileX].getTextureId ();
}

/// \fn Layer:setTile(int x, int y, int tile)
/// \breif Set the new tile texture.
void Layer::setTile(int x, int y, int tile)
{
   if ( x >= 0 && y >= 0 && x < width && y < height )
   {
      field[y][x].setTextureId(tile);

      dirty = true;
   }
}

/// \fn Layer::resize(int newwidth, int newheight)
/// \brief Resize the layer to the new dimensions.
void Layer::resize(int newwidth, int newheight)
{
   TileRow* pnewrows = createTileRows(newwidth, newheight);
   int size = MIN(newheight, height);
   for ( int y = 0; y < size; ++y )
   {
      pnewrows[y] = field[y];
   }

   delete[] field;
   field = pnewrows;

   width = newwidth;
   height = newheight;

   prepare();

   dirty = true;
}
