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

#include "core/log/log.h"

#include "animation.h"
#include "actor.h"

Animator* Animator::construct(const TiXmlElement& xmlObject, Actor& actor)
{
   Animator* panimator = NULL;
   const TiXmlElement* pXmlAnimation = static_cast<const TiXmlElement*>(xmlObject.FirstChild("animations"));
	if ( pXmlAnimation != NULL )
   {
      panimator = new Animator();
      panimator->loadFromXML(*pXmlAnimation, actor);
   }

   return panimator;
}

Animator::Animator():
   mTextureCoords(),
   mAnimations(),
   mAnimationSpeed(0),
   mAnimationDelta(0),
   mAnimFrameWidth(0),
   mAnimFrameCount(0),
   mCurrentAnimation(0),
   mAnimIndex(0),
   mAnimFrame(0),
   mAnimChanged(false)
{
}

Animator::~Animator()
{
}

bool Animator::loadFromXML(const TiXmlElement& xmlanimation, Actor& actor)
{
	// try to load the animation sequences
   int animCount = 0;
	if ( xmlanimation.QueryIntAttribute("count", &animCount) != TIXML_SUCCESS )
   {
		// found an animation object without animations, better use Object class then
      Log::getInstance().warning("Found animatable object without animations.");
	}
   
   if ( animCount > 0 )
   {
		// query the animation speed (in mm)
		if ( xmlanimation.QueryFloatAttribute("speed", &mAnimationSpeed) != TIXML_SUCCESS )
			mAnimationSpeed = 100;

      mAnimationSpeed /= 1000.0f;

		// allocate a new animationset
		mAnimations = SharedPtr<AnimationSet>(new AnimationSet());
      if ( !mAnimations.hasPointer() || !parseAnimations(xmlanimation, animCount) )
			return false;

      // query the frame count
      if ( xmlanimation.QueryIntAttribute("frames", (int*)mAnimFrameCount) != TIXML_SUCCESS)
         determineFrameCount();
	}
	
   const Texture& texture = actor.getTexture();
   const Vector& size     = actor.getSize();
   mAnimFrameWidth = texture.getWidth() / size.x;
   mTextureCoords.generateFromTexture(texture, size.x, size.y, mAnimFrameCount);

	return true;
}

bool Animator::parseAnimations(const TiXmlElement& xmlanimations, int count)
{
   char buffer[50];

   for ( int i = 1; i <= count; i++ )
   {
		sprintf (buffer, "anim%d", i);
		const TiXmlElement* pxmlAnim = static_cast<const TiXmlElement*>(xmlanimations.FirstChild(buffer));
		if ( pxmlAnim == NULL )
      {
         Log::getInstance().error("AnimObject.create: can not find %s", buffer);
			return false;
		}

      // parse the comma seperated string
		const TiXmlText* xmlValue = static_cast<const TiXmlText*>(pxmlAnim->FirstChild());
		Animation *panim = new Animation();
		getAnimations().add(panim);

		// now parse the animation sequence
		parseAnimation(xmlValue->Value(), panim);
		panim->add(-1);
	}

   return true;
}

/// \fn AnimObject::parseAnimation (const char* animation)
void Animator::parseAnimation (const char* sequence, Animation *panimation)
{
	char number[10] = "";
	int length = strlen (sequence);
	int anim = 0, j = 0;

	// parse the sequence
	for ( int i = 0; i < length; ++i )
   {
		if (isdigit (sequence[i]))
			number[j++] = sequence[i];
		else
      {
			// save the number and skip trailing spaces
			panimation->add (atoi (number));
			while (isspace (sequence[i]))
            i++;

         i--;
			j = 0;
		}
	}

	// see if there is still a number in the buffer
	if (j > 0)
      panimation->add (atoi (number));
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

void Animator::animate(float delta)
{
   mAnimationDelta += delta;
   if ( mAnimationDelta >= mAnimationSpeed )
   {
      nextFrame();

      mAnimationDelta -= mAnimationSpeed;
   }
}

/// \fn AnimObject::nextFrame()
void Animator::nextFrame()
{
   if ( getAnimations().size() > 0 )
   {
	   // move to next frame
	   mAnimIndex++;
      const Animation& animation = getAnimations()[mCurrentAnimation];
	   mAnimFrame = animation[mAnimIndex];

	   // wrap if neccessary
	   if (mAnimFrame == -1)
      {
		   mAnimIndex = 0;
		   mAnimFrame = animation[mAnimIndex];
	   }
   }
}

int Animator::getAnimation() const
{
   return mCurrentAnimation + 1;
}

void Animator::setAnimation(int animation)
{
   animation--;

	if ( animation != mCurrentAnimation )
   {
		mCurrentAnimation = animation;
		mAnimIndex = 0;
      mAnimFrame = 0;

      mAnimChanged = true;
	}
}

const TextureCoordinate& Animator::getTextureCoordinate()
{
   return mTextureCoords[mAnimFrame];
}
