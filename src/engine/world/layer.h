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
#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <fstream>

#include "core/math/vector.h"
#include "core/defines.h"

#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/effect.h"

#include "tileset.h"
#include "layertype.h"

#define COLLISION_BOTTOM			1
#define COLLISION_RIGHT				2
#define COLLISION_LEFT				4
#define COLLISION_TOP				8

#define COLLIDED_HORZ				1
#define COLLIDED_VERT				2
#define COLLIDED_GRAFITY			4

class Point;
class TileRow;
class VertexBuffer;

/**
@author Jeroen Broekhuizen
\brief This class represents one layer in your world. A game can have an unlimitted number of layers.

A layer is a piece of the world which you can load and render on you screen. It can represent for example a
sunset background or whatever your like. A layer uses only one texture as its tileset, which should contain
at least one sub-texture which can be rendered on the tiles.
*/
class Layer
{
public:
                  Layer();
   virtual        ~Layer();

 // Construction
   bool           create(const std::string& name, int width, int height, const std::string& effect);
   void           release();

 // pure virtuals
           bool   prepare();
   virtual bool   prepare(int screenWidth, int screenHeight) = 0;
   virtual void   draw() = 0;
   virtual void   drawHighlight(const Vector& point) = 0;

 // get/set interface
   const std::string& getName() const;
   void               setName(const std::string& name);

   virtual LayerType  getType() const;

   virtual int            getTile(int x, int y) const;
   virtual void           setTile(int x, int y, int tile);

   Vector         getScrollSpeed() const;
   void           setScrollSpeed(float x, float y);
   Vector         getScroll() const;
   void           setScroll(float x, float y);

   Vector         getDimensions() const;

   void           enableTileAnimation( bool enable );
   bool           isTileAnimating() const;

   const TileSet& tileset() const;
         TileSet& tileset();

   int            tilecount() const;
   int            tilewidth() const;
   int            tileheight() const;

   const Vector&  tilecoordinates(int index) const;

   float          getTexTileWidth() const { return texTileWidth; }
   float          getTexTileHeight() const { return texTileHeight; }

   const Effect&        getEffect() const;
   const std::string&   getEffectFile() const;

 // Operations
   Vector         layerToScreen( const Vector& vec ) const;
   Vector         screenToLayer( const Vector& vec ) const;

   Vector         getScrollArea() const;

   virtual Point  pointToTile(const Point& point) = 0;

   void           scroll(float x, float y);
   void           resize(int newwidth, int newheight);
   void           update(float delta);

   void           calculateScrollSpeed(const Vector& area, int screenWidth, int screenHeight);

protected:
 // pure virtuals
   virtual TileRow*  createTileRows(int width, int height) = 0;

 // operations
   VertexBuffer*  createVertexBuffer(int width, int height, int vertexcount);
   void           setVertex(float** data, float x, float y, float texX, float texY);

   int tileWidth, tileHeight, tileCount;
   float texTileWidth, texTileHeight;
   int maxTilesOnRow;
   int width, height;
   float xscroll, yscroll;
   float xscrollMax, yscrollMax;
   float scrollSpeedX, scrollSpeedY;
   int maxTilesX, maxTilesY;
   int verts, verts_to_render;
   bool animateTiles;
   bool dirty;

   std::string name, effectFile;
   Effect effect;
   TileSet mTileSet;

   TileRow* field;
   VertexBuffer* vb;
   Vector* texcoordLookup;
   Texture diffuseMap, normalMap, normalizeCube;
};

#ifdef JENGINE_INLINE
#  include "layer.inl"
#endif

#endif
