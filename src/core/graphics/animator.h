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
#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include <vector>

#include "core/smartptr/sharedptr.h"
#include "core/core_base.h"

struct Size;

namespace Graphics
{
   class Texture;
   class TextureCoordinate;
}

namespace c2d
{
   class AnimationState;

   class CORE_API Animator
   {
   public:
      using Animation = std::vector<int>;

      enum AnimationType { eIdle, eWalking, eRunning, eJumping, eSliding, eDying, eInvalid };

      Animator();
      ~Animator();

    // get/set
      void setAnimationSpeed(float milliseconds) {
         mAnimationSpeed = milliseconds;
      }
      
    // operations
      void add(AnimationType type, Animation&& animation);
      Animation& emplaceAnimation();
      void flip();

    // Animation
      void setAnimation(AnimationState& state, int index);
      bool animate(AnimationState& state) const;

   private:
      using Animations = std::vector<Animation>;

    // query
      bool canAnimate(AnimationState& state) const;

    // animation
      void nextFrame(AnimationState& state) const;
      
    // data
      Animations  mAnimations;
	   float       mAnimationSpeed;
      int         mAnimFrameCount;
   };

   typedef SharedPtr<Animator> AnimatorPtr;
}

#endif
