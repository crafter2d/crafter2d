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

#include "core/smartptr/sharedptr.h"
#include "core/core_base.h"

#include "animationset.h"
#include "texturecoordlookup.h"

struct Size;
class TiXmlElement;

namespace Graphics
{
   class AnimationState;
   class Texture;
   class TextureCoordinate;

   class CORE_API Animator
   {
   public:
      static Animator* construct(const TiXmlElement& xmlObject);

      Animator();
      ~Animator();

    // operations
      void initialize(const Texture& texture, const Size& meshsize);

    // Animation
      bool animate(AnimationState& state) const;

      int  getAnimation() const;
      bool setAnimation(int animation);

      const TextureCoordinate& getTextureCoordinate(const AnimationState& state) const;

   private:
    // get/set
      const AnimationSet&  getAnimations() const;
            AnimationSet&  getAnimations();

    // query
      bool canAnimate(AnimationState& state) const;

    // animation
      void nextFrame(AnimationState& state) const;

    // parsing
      bool loadFromXML(const TiXmlElement& xmlanimation);
      void parseAnimations(const TiXmlElement& xmlanimator);
      void parseAnimation(const char* sequence, Animation *animation);

      void determineFrameCount();

      TextureCoordLookup mTextureCoords;
      AnimationSet*      mpAnimations;
	   float    mAnimationSpeed;
	   float    mAnimationDelta;
      float    mAnimFrameWidth;
      int      mAnimFrameCount;
   };

   typedef SharedPtr<Animator> AnimatorPtr;
}

#ifdef JENGINE_INLINE
#  include "animator.inl"
#endif

#endif
