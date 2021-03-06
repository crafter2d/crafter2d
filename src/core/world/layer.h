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

#include <fstream>

#include "core/core_base.h"
#include "core/graphics/effect.h"
#include "core/math/vector.h"
#include "core/math/matrix4.h"
#include "core/math/size.h"
#include "core/defines.h"
#include "core/string/string.h"

#include "tilefield.h"
#include "tileset.h"
#include "layertype.h"

#define COLLISION_BOTTOM			1
#define COLLISION_RIGHT				2
#define COLLISION_LEFT				4
#define COLLISION_TOP				8

#define COLLIDED_HORZ				1
#define COLLIDED_VERT				2
#define COLLIDED_GRAFITY			4

namespace Graphics
{
   class Device;
   class Viewport;
   class RenderContext;
   class IndexBuffer;
   class VertexBuffer;
   class UniformBuffer;
   class Viewport;
};

namespace c2d
{
   class Point;
}

struct LayerDefinition
{
   int width;
   int height;
   String name;
   String effect;
   String tileset;
};

/**
@author Jeroen Broekhuizen
\brief This class represents one layer in your world. A game can have an unlimitted number of layers.

A layer is a piece of the world which you can load and render on you screen. It can represent for example a
sunset background or whatever your like. A layer uses only one texture as its tileset, which should contain
at least one sub-texture which can be rendered on the tiles.
*/
class CORE_API Layer
{
public:
                  Layer();
   virtual        ~Layer();

 // Construction
   virtual bool create(LayerDefinition* pdefinition);
   virtual bool initialize(Graphics::Device& device);
   void         release();

 // pure virtuals
   virtual void   draw(Graphics::RenderContext& context) = 0;
   virtual void   drawFront(Graphics::RenderContext& context) = 0;

 // get/set interface
   const String& getName() const;
   void          setName(const String& name);

   virtual LayerType  getType() const;

   virtual uint8_t getTile(LayerLevel level, int x, int y) const;
   virtual void    setTile(LayerLevel level, int x, int y, uint8_t tile);

   const LayerDefinition& getDefinition() const;
   void                   setDefinition(LayerDefinition* pdefinition);

   const TileField& getTileField() const;
         TileField& getTileField();

   Vector         getScrollSpeed() const;
   void           setScrollSpeed(float x, float y);
   Vector         getScroll() const;
   void           setScroll(float x, float y);

   int            getWidth() const;
   int            getHeight() const;

   Size getPixelSize() const { return Size(getWidth() * mTileWidth, getHeight() * mTileHeight); }

   void           enableTileAnimation( bool enable );
   bool           isTileAnimating() const;

   const TileSet& tileset() const;
         TileSet& tileset();
   void           setTileSet(TileSet* ptileset);

   int            tileWidth() const { return mTileWidth; }
   int            tileHeight() const { return mTileHeight; }
   
   const Graphics::Effect& getEffect() const;
         Graphics::Effect& getEffect();

 // Operations
   Vector         layerToScreen( const Vector& vec ) const;
   Vector         screenToLayer( const Vector& vec ) const;

   Vector         getScrollArea() const;

   virtual c2d::Point pointToTile(const c2d::Point& point) = 0;

   void           scroll(Graphics::RenderContext& context, float x, float y);
   void           update(float delta);

   void           calculateScrollSpeed(const Vector& area);
 
 // editing
   void resize(int width, int height);

 // notifications
   virtual void onViewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport);

protected:
 // operations
   void           setVertex(float** data, float x, float y, float texX, float texY);

   int mTileWidth, mTileHeight;
   float xscroll, yscroll;
   float xscrollMax, yscrollMax;
   float scrollSpeedX, scrollSpeedY;
   int maxTilesX, maxTilesY;
   int verts_to_render, verts_to_render_front;
   bool animateTiles;
   bool dirty;

   TileSet* mpTileSet;
   TileField mTileMap;

   Graphics::VertexBuffer* vb;
   Graphics::VertexBuffer* pfrontvb;
   Graphics::IndexBuffer* ib;
   Graphics::UniformBuffer* ub;

private:
   struct ConstantBuffer
   {
      Matrix4 projection;
      Matrix4 world;
      Matrix4 object;
   };

 // operations
   bool createBuffers(Graphics::Device& device, int width, int height);
   bool createUniformBuffers(Graphics::Device& device);

 // members
   LayerDefinition*  mpDefinition;
   Graphics::Effect* mpEffect;
   ConstantBuffer    mConstants;
};

#ifdef JENGINE_INLINE
#  include "layer.inl"
#endif

#endif
