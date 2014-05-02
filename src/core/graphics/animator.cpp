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
#include "animator.h"
#ifndef JENGINE_INLINE
#  include "animator.inl"
#endif

#include "core/graphics/texture.h"
#include "core/log/log.h"
#include "core/math/size.h"
#include "core/smartptr/autoptr.h"

#include "animation.h"
#include "animation/animationstate.h"

namespace Graphics
{

Animator::Animator():
   mTextureCoords(),
   mAnimations(),
   mAnimationSpeed(0),
   mAnimationDelta(0),
   mAnimFrameWidth(0),
   mAnimFrameCount(0)
{
}

Animator::~Animator()
{
}

// - Operations

void Animator::initialize(const Texture& texture, const Size& meshsize)
{
   determineFrameCount();
   mAnimFrameWidth = texture.getWidth() / meshsize.width;
   mTextureCoords.generateFromTexture(texture, meshsize, mAnimFrameCount);
}

void Animator::addAnimation(int start, int length)
{
   Animation* panimation = new Animation();
   panimation->generate(start, length);
   mAnimations.add(panimation);
}

void Animator::determineFrameCount()
{
   mAnimFrameCount = 0;
   for ( int anim = 0; anim < getAnimations().size(); ++anim )
   {
      const Animation& animation = getAnimations()[anim];
      for ( Animation::size_type frameindex = 0; frameindex < animation.size(); ++frameindex )
      {
         int frame = animation[frameindex];
         if ( frame > mAnimFrameCount )
            mAnimFrameCount = frame;
      }
   }

   // make sure that the count is max + 1 (otherwise the last frame gets no texture coordinates.
   ++mAnimFrameCount;
}

//--------------
//- Animation

bool Animator::canAnimate(AnimationState& state) const
{
   return !mAnimations.isEmpty() && mAnimations[state.mAnimation].size() > 1;
}

bool Animator::animate(AnimationState& state) const
{
   if ( canAnimate(state) && state.mDelta >= mAnimationSpeed )
   {
      nextFrame(state);

      state.mDelta = 0;

      return true;
   }

   return false;
}

/// \fn AnimObject::nextFrame()
void Animator::nextFrame(AnimationState& state) const
{
   const Animation& animation = mAnimations[state.mAnimation];

   if ( ++state.mAnimFrame >= animation.size() )
   {
      state.mAnimFrame = 0;
   }

   state.mTexIndex = animation[state.mAnimFrame];
}

const TextureCoordinate& Animator::getTextureCoordinate(const AnimationState& state) const
{
   return mTextureCoords[state.mTexIndex];
}

} // namespace Graphics
