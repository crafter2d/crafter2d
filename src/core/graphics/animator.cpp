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

#include <tinyxml.h>

#include "core/graphics/texture.h"
#include "core/log/log.h"
#include "core/math/size.h"
#include "core/smartptr/autoptr.h"

#include "animation.h"
#include "animation/animationstate.h"

namespace Graphics
{

Animator* Animator::construct(const TiXmlElement& xmlAnimation)
{
   AutoPtr<Animator> animator = new Animator();
   if ( animator->loadFromXML(xmlAnimation) )
   {
      return animator.release();
   }

   return NULL;
}

Animator::Animator():
   mTextureCoords(),
   mpAnimations(NULL),
   mAnimationSpeed(0),
   mAnimationDelta(0),
   mAnimFrameWidth(0),
   mAnimFrameCount(0)
{
}

Animator::~Animator()
{
   delete mpAnimations;
   mpAnimations = NULL;
}

// - Operations

void Animator::initialize(const Texture& texture, const Size& meshsize)
{
   mAnimFrameWidth = texture.getWidth() / meshsize.width;
   mTextureCoords.generateFromTexture(texture, meshsize, mAnimFrameCount);
}

bool Animator::loadFromXML(const TiXmlElement& xmlanimator)
{
   // query the animation speed (in mm)
	if ( xmlanimator.QueryFloatAttribute("speed", &mAnimationSpeed) != TIXML_SUCCESS )
		mAnimationSpeed = 100;
   mAnimationSpeed /= 1000.0f;
   
   // allocate the memory set
   mpAnimations = new AnimationSet();
   if ( mpAnimations == NULL )
   {
      return false;
   }

   parseAnimations(xmlanimator);

	return true;
}

void Animator::parseAnimations(const TiXmlElement& xmlanimator)
{
   const TiXmlElement* pxmlanimation = static_cast<const TiXmlElement*>(xmlanimator.FirstChild("anim"));
   while ( pxmlanimation != NULL )
   {
		Animation *panim = new Animation();
		getAnimations().add(panim);

		// now parse the animation sequence
      const TiXmlText* xmlValue = static_cast<const TiXmlText*>(pxmlanimation->FirstChild());
		parseAnimation(xmlValue->Value(), panim);

      pxmlanimation = static_cast<const TiXmlElement*>(xmlanimator.IterateChildren ("anim", pxmlanimation));
   }

   determineFrameCount();
}

/// \fn AnimObject::parseAnimation (const char* animation)
void Animator::parseAnimation (const char* sequence, Animation *panimation)
{
	char number[10] = "";
	int length = strlen (sequence);
	int j = 0;

	// parse the sequence
	for ( int i = 0; i < length; ++i )
   {
	   if (isdigit (sequence[i]))
		   number[j++] = sequence[i];
      else
      {
         // save the number and skip trailing spaces
         panimation->add(atoi(number));
         while (isspace (sequence[i]))
            i++;

         i--;
         j = 0;
      }
	}

	// see if there is still a number in the buffer
	if (j > 0)
      panimation->add(atoi(number));
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
   return !mpAnimations->isEmpty() && getAnimations()[state.mAnimation].size() > 1;
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
   const Animation& animation = getAnimations()[state.mAnimation];

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
