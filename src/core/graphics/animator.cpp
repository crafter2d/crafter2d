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

#include "core/graphics/texture.h"
#include "core/log/log.h"
#include "core/math/size.h"

#include "animation/animationstate.h"

namespace c2d
{

Animator::Animator():
   mAnimations(),
   mAnimationSpeed(0),
   mAnimFrameCount(0)
{
}

Animator::~Animator()
{
}

// - Operations

void Animator::add(AnimationType type, Animation&& animation)
{
   mAnimations[type] = std::move(animation);
}

//--------------
//- Animation

bool Animator::canAnimate(AnimationState& state) const
{
   return !mAnimations.empty() && mAnimations[state.mAnimation].size() > 1;
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

void Animator::setAnimation(AnimationState& state, int index)
{
   if ( state.mAnimation != index )
   {
      const Animation& animation = mAnimations[index];

      state.mDelta = 0.0f;
      state.mAnimation = index;
      state.mAnimFrame = 0;
      state.mTileIndex = animation[0];
   }
}

/// \fn AnimObject::nextFrame()
void Animator::nextFrame(AnimationState& state) const
{
   const Animation& animation = mAnimations[state.mAnimation];

   if ( ++state.mAnimFrame >= animation.size() )
   {
      state.mAnimFrame = 0;
   }

   state.mTileIndex = animation[state.mAnimFrame];
}

} // namespace c2d
