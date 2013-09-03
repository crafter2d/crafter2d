/***************************************************************************
 *   Copyright (C) 2013 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "sprite.h"

#include "core/graphics/texture.h"
#include "core/graphics/animator.h"

#include "spritedefinition.h"

namespace Graphics
{

Sprite::Sprite(const SpriteDefinition& definition):
   mDefinition(definition),
   mAnimState(),
   mTexture(),
   mTexCoordinate(),
   mTransform(),
   mHalfSize()
{
}

// - Query

const Size& Sprite::getHalfSize() const
{
   return mHalfSize;
}

const Texture& Sprite::getTexture() const
{
   return mDefinition.getTexture();
}

const TextureCoordinate& Sprite::getTextureCoordinate() const
{
   return mTexCoordinate;
}

void Sprite::setTextureCoordinate(const TextureCoordinate& coordinate)
{
   mTexCoordinate = coordinate;
}

const XForm& Sprite::getTransform() const
{
   return mTransform;
}

void Sprite::setTransform(const XForm& xform)
{
   mTransform = xform;
}

// - Operations

bool Sprite::initialize(Device& device)
{
   mHalfSize = mDefinition.getSize() / 2.0f;

   return true;
}

void Sprite::update(float delta)
{
   if ( mDefinition.hasSpriteAnimator() )
   {
      mAnimState.update(delta);
      if ( mDefinition.getSpriteAnimator().animate(mAnimState) )
      {
         mTexCoordinate = mDefinition.getSpriteAnimator().getTextureCoordinate(mAnimState);
      }
   }
}

} // Graphics
