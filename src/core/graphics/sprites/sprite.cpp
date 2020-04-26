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

#include "core/graphics/animator.h"

#include "spritedefinition.h"

namespace c2d
{

   Sprite::Sprite(SpriteDefinition& definition) :
      mDefinition(definition),
      mAnimState(),
      mTransform(),
      mHalfSize(),
      mTile(definition.getTile()),
      mSortOrder(0),
      mFlipped(false)
   {
   }

   Sprite::Sprite(Sprite&& other) noexcept :
      mDefinition(other.mDefinition),
      mAnimState(other.mAnimState),
      mTransform(other.mTransform),
      mHalfSize(other.mHalfSize),
      mTile(other.mTile),
      mSortOrder(other.mSortOrder),
      mFlipped(other.mFlipped)
   {
   }

   Sprite::~Sprite()
   {
   }

   Sprite& Sprite::operator=(Sprite&& other) noexcept
   {
      if ( this != &other )
      {
         mDefinition = std::move(other.mDefinition);
         mAnimState = other.mAnimState;
         mTransform = other.mTransform;
         mHalfSize = other.mHalfSize;
         mTile = other.mTile;
         mSortOrder = other.mSortOrder;
         mFlipped = other.mFlipped;
      }
      return *this;
   }

   // - Operations

   bool Sprite::initialize()
   {
      mHalfSize = mDefinition.getSize() / 2.0f;

      setAnimation(0);

      return true;
   }

   void Sprite::update(float delta)
   {
      if ( mDefinition.hasSpriteAnimator() )
      {
         mAnimState.update(delta);
         if ( mDefinition.getSpriteAnimator().animate(mAnimState) )
         {
            mTile = mAnimState.getTileIndex();
         }
      }
   }

   void Sprite::setAnimation(int index)
   {
      if ( mDefinition.hasSpriteAnimator() )
      {
         mDefinition.getSpriteAnimator().setAnimation(mAnimState, index);
         
         mTile = mAnimState.getTileIndex();
      }
   }

   void Sprite::flip()
   {
      mFlipped = !mFlipped;
   }

} // namespace c2d
