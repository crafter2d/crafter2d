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

#include "animation.h"
#include "console.h"
#include "object.h"

Animator* Animator::construct(TiXmlElement* pXmlObject, Object& object)
{
   Animator* panimator = NULL;
   TiXmlElement* pXmlAnimation = (TiXmlElement*)pXmlObject->FirstChild("animations");
	if ( pXmlAnimation != NULL )
   {
      panimator = new Animator();
      panimator->loadFromXML(pXmlAnimation, object);
   }

   return panimator;
}

Animator::Animator():
   mTextureCoords(),
   mAnimations(),
   mAnimationSpeed(0),
   mAnimationDelta(0),
   _animFrameWidth(0),
   _animFrameCount(0),
   mCurrentAnimation(0),
   animIndex(0),
   animFrame(0),
   _animChanged(false)
{
}

Animator::~Animator()
{
}

bool Animator::loadFromXML(TiXmlElement* pXmlAnimation, Object& object)
{
	// try to load the animation sequences
   int animCount = 0;
	if ( pXmlAnimation->QueryIntAttribute("count", &animCount) == TIXML_SUCCESS )
   {
		if ( animCount > 0 )
      {
			// query the animation speed (in mm)
			if (pXmlAnimation->QueryFloatAttribute("speed", &mAnimationSpeed) != TIXML_SUCCESS)
				mAnimationSpeed = 100;

         mAnimationSpeed /= 1000.0f;

			// allocate a new animationset
			mAnimations = new AnimationSet ();
         if ( !mAnimations.valid() || !parseAnimations(pXmlAnimation, animCount) )
				return false;

         // query the frame count
         if ( pXmlAnimation->QueryIntAttribute("frames", (int*)&_animFrameCount) != TIXML_SUCCESS)
            determineFrameCount();

			// we are using it
			mAnimations->addRef ();
		}
	}
	else
   {
		// found an animation object without animations, better use Object class then
      Console::getInstance().print ("Warning: found animatable object without animations.");
	}

   const Texture& texture = object.getTexture();
   const Vector& size     = object.getSize();
   _animFrameWidth = texture.getWidth() / size.x;
   mTextureCoords.generateFromTexture(texture, size.x, size.y, _animFrameCount);

	return true;
}

bool Animator::parseAnimations(TiXmlElement* pxmlAnimations, int count)
{
   char buffer[50];

   for ( int i = 1; i <= count; i++ )
   {
		sprintf (buffer, "anim%d", i);
		TiXmlElement* pxmlAnim = (TiXmlElement*)pxmlAnimations->FirstChild(buffer);
		if ( pxmlAnim == NULL )
      {
         Console::getInstance().printf ("AnimObject.create: can not find %s", buffer);
			return false;
		}
		else
      {
			// parse the comma seperated string
			TiXmlText* xmlValue = (TiXmlText*)pxmlAnim->FirstChild();
			Animation *panim = new Animation();
			getAnimations().add(panim);

			// now parse the animation sequence
			parseAnimation(xmlValue->Value(), panim);
			panim->add(-1);
		}
	}

   return true;
}

/// \fn AnimObject::parseAnimation (const char* animation)
void Animator::parseAnimation (const char* sequence, Animation *panimation)
{
	char number[10] = "";
	Uint32 length = (Uint32)strlen (sequence);
	Uint32 anim = 0, j = 0;

	// parse the sequence
	for ( Uint32 i = 0; i < length; ++i )
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
   _animFrameCount = 0;
   for ( int anim = 0; anim < getAnimations().size(); ++anim )
   {
      const Animation& animation = getAnimations()[anim];
      for ( Animation::size_type frameindex = 0; frameindex < animation.size(); ++frameindex )
      {
         int frame = animation[frameindex];
         if ( frame > _animFrameCount )
            _animFrameCount = frame;
      }
   }

   // make sure that the count is max + 1 (otherwise the last frame gets no texture coordinates.
   ++_animFrameCount;
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
	   animIndex++;
      const Animation& animation = getAnimations()[mCurrentAnimation];
	   animFrame = animation[animIndex];

	   // wrap if neccessary
	   if (animFrame == -1)
      {
		   animIndex = 0;
		   animFrame = animation[animIndex];
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
		animIndex = 0;
      animFrame = 0;

      _animChanged = true;
	}
}

const TextureCoordinate& Animator::getTextureCoordinate()
{
   return mTextureCoords[animFrame];
}
