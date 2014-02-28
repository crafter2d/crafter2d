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
#include "core/defines.h"

INLINE const String& Layer::getName() const
{
   return mpDefinition->name;
}

INLINE void Layer::setName(const String& name)
{
   mpDefinition->name = name;
}

/// \fn Layer::getWidth()
/// \brief Returns the number of tiles that fit horizontally in this layer.
INLINE int Layer::getWidth() const
{
   return mpDefinition->width;
}

/// \fn Layer::getHeight()
/// \brief Returns the number of tiles that fit vertically in this layer.
INLINE int Layer::getHeight() const
{
   return mpDefinition->height;
}

INLINE const LayerDefinition& Layer::getDefinition() const
{
   ASSERT_PTR(mpDefinition);
   return *mpDefinition;
}

INLINE void Layer::setDefinition(LayerDefinition* pdefinition)
{
   mpDefinition = pdefinition;
}

/// \fn Layer::setVertex ()
/// \brief Inserts a new vertex into the vertex buffer
INLINE void Layer::setVertex (float** data, float xpos, float ypos, float texX, float texY)
{
	(*data)[0] = xpos;
	(*data)[1] = ypos;
	(*data)[2] = texX;
	(*data)[3] = texY;
	(*data) += 4;
}

/// \fn Layer::enableTileAnimation(bool enable)
/// \brief Enable or disable tile animation.
INLINE void Layer::enableTileAnimation(bool enable)
{
   animateTiles = enable;
}

/// \fn Layer::isTileAnimating()
/// \brief Test if the tiles are animating.
INLINE bool Layer::isTileAnimating() const
{
   return animateTiles;
}

/// \fn Layer::getEffect()
/// \brief Return the effect for this layer.
INLINE const Graphics::Effect& Layer::getEffect() const
{
   ASSERT_PTR(mpEffect)
   return *mpEffect;
}

INLINE Graphics::Effect& Layer::getEffect()
{
   ASSERT_PTR(mpEffect)
   return *mpEffect;
}

/// \fn Layer::layerToScreen( const Vector& vec ) const
INLINE Vector Layer::layerToScreen( const Vector& vec ) const
{
	return Vector(vec.x - xscroll, vec.y - yscroll);
}

/// \fn Layer::screenToLayer( const Vector& vec ) const
INLINE Vector Layer::screenToLayer( const Vector& vec ) const
{
   return Vector(vec.x + xscroll, vec.y + yscroll);
}

/// \fn Layer::getScroll()
/// \returns values that the view scrolled
INLINE Vector Layer::getScroll () const
{
	return Vector(xscroll, yscroll);
}

INLINE void Layer::setScroll(float x, float y)
{
   if ( x != xscroll || y != yscroll )
   {
      xscroll = x;
      yscroll = y;

      dirty = true;
   }
}

INLINE const TileSet& Layer::tileset() const
{
   ASSERT_PTR(mpTileSet);
   return *mpTileSet;
}

INLINE TileSet& Layer::tileset()
{
   ASSERT_PTR(mpTileSet);
   return *mpTileSet;
}

INLINE void Layer::setTileSet(TileSet* ptileset)
{
   mpTileSet = ptileset;
}

INLINE const TileField& Layer::getTileField() const
{
   return mTileMap;
}

INLINE TileField& Layer::getTileField()
{
   return mTileMap;
}

INLINE int Layer::tilecount() const
{
   return tileCount;
}

INLINE int Layer::tilewidth() const
{
   return tileWidth;
}

INLINE int Layer::tileheight() const
{
   return tileHeight;
}

INLINE const Vector& Layer::tilecoordinates(int index) const
{
   assert(texcoordLookup != NULL && "Texture coordinate lookup table not initialized.");
   return texcoordLookup[index];
}

INLINE Vector Layer::getScrollSpeed() const
{
   return Vector(scrollSpeedX, scrollSpeedY);
}

INLINE void Layer::setScrollSpeed(float x, float y)
{
   scrollSpeedX = x;
   scrollSpeedY = y;
}
