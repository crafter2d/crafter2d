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
#include "animatable.h"
#ifndef JENGINE_INLINE
#  include "animatable.inl"
#endif

#include <SDL/SDL.h>
#include <GL/GLee.h>
#include <tinyxml.h>

#include "object.h"
#include "console.h"
#include "nodevisitor.h"

/**************************************************************
 * AnimationSet class
 */

AnimationSet::~AnimationSet()
{
   destroy();
}

void AnimationSet::destroy()
{
   while (!animations.empty())
      remove(animations[0]);
}

void AnimationSet::remove(Animation* animation)
{
   Animations::iterator it = find(animation);
   if (it != animations.end()) {
      animations.erase(it);
   }
}

/**************************************************************
 * AnimObject class
 */

IMPLEMENT_REPLICATABLE(AnimObjectId, AnimObject, Object)

AnimObject::AnimObject():
   Object(),
   _animations(),
   _textureCoords(),
   animSpeed(100),
	animLast(0),
   _animChanged(false),
   _animFrameWidth(1.0f),
	animCur(0),
	animIndex(0),
	animFrame(0)
{
}


AnimObject::~AnimObject()
{
}

bool AnimObject::load(TiXmlDocument& doc)
{
   int animCount = 0;

	// load in the object variables
	if ( !Object::load(doc) )
		return false;

   _animFrameWidth = texture->getWidth() / width;

	TiXmlElement* xmlObject = (TiXmlElement*)doc.FirstChild("object");

	// try to load the animation sequences
	TiXmlElement* xmlAnimation = (TiXmlElement*)xmlObject->FirstChild("animations");
	if (xmlAnimation && xmlAnimation->QueryIntAttribute("count", &animCount) == TIXML_SUCCESS) {
		if (animCount > 0) {
			char buffer[50];

			// query the animation speed
			if (xmlAnimation->QueryIntAttribute ("speed", (int*)&animSpeed) != TIXML_SUCCESS)
				animSpeed = 100;

			// allocate a new animationset
			_animations = new AnimationSet ();
         if ( !_animations.valid() )
				return false;

			// parse all animations
			for (int i = 1; i <= animCount; i++) {
				sprintf (buffer, "anim%d", i);
				TiXmlElement* xmlAnim = (TiXmlElement*)xmlAnimation->FirstChild(buffer);
				if (xmlAnim == NULL) {
               Console::getInstance().printf ("AnimObject.create: can not find %s", buffer);
					return false;
				}
				else {
					// parse the comma seperated string
					TiXmlText* xmlValue = (TiXmlText*)xmlAnim->FirstChild();
					Animation *anim = new Animation;
					getAnimations().add (anim);

					// now parse the animation sequence
					parseAnimation (xmlValue->Value(), anim);
					anim->add (-1);
				}
			}

         // query the frame count
         if ( xmlAnimation->QueryIntAttribute("frames", (int*)&_animFrameCount) != TIXML_SUCCESS)
            determineFrameCount();

			// we are using it
			_animations->addRef ();
		}
	}
	else {
		// found an animation object without animations, better use Object class then
      Console::getInstance().print ("Warning: found animatable object without animations.");
	}

   _textureCoords.generateFromTexture(*texture, width, height, _animFrameCount);

	return true;
}

/*!
    \fn AnimObject::parseAnimation (const char* animation)
 */
void AnimObject::parseAnimation (const char* sequence, Animation *animation)
{
	char number[10] = "";
	Uint32 length = (Uint32)strlen (sequence);
	Uint32 anim = 0, j = 0;

	// parse the sequence
	for ( Uint32 i = 0; i < length; ++i )
   {
		if (isdigit (sequence[i]))
			number[j++] = sequence[i];
		else {
			// save the number and skip trailing spaces
			animation->add (atoi (number));
			while (isspace (sequence[i])) i++;

         i--;
			j = 0;
		}
	}
	// see if there is still a number in the buffer
	if (j > 0) animation->add (atoi (number));
}

void AnimObject::determineFrameCount()
{
   _animFrameCount = 0;
   for ( int anim = 0; anim < getAnimations().size(); ++anim )
   {
      const Animation& animation = getAnimations()[anim];
      for ( int frameindex = 0; frameindex < animation.size(); ++frameindex )
      {
         int frame = animation[frameindex];
         if ( frame > _animFrameCount )
            _animFrameCount = frame;
      }
   }

   // make sure that the count is max + 1 (otherwise the last frame gets no texture coordinates.
   ++_animFrameCount;
}

void AnimObject::setAnimation (int anim)
{
	//assert (anim >= 0 && anim < anims->size() && "AnimObject.setAnimation: invalid animation index");
	if ( anim != animCur )
   {
		animCur = anim;
		animIndex = 0;

      _animChanged = true;
	}
}

/*!
    \fn AnimObject::animate ()
 */
void AnimObject::animate (Uint32 tick)
{
	if (tick - animLast >= animSpeed)
   {
		nextFrame ();
		animLast = SDL_GetTicks ();
	}
}

/*!
    \fn AnimObject::nextFrame ()
 */
void AnimObject::nextFrame ()
{
   if ( getAnimations().size() > 0 )
   {
	   // move to next frame
	   animIndex++;
      const Animation& animation = getAnimations()[animCur - 1];
	   animFrame = animation[animIndex];

	   // wrap if neccessary
	   if (animFrame == -1)
      {
		   animIndex = 0;
		   animFrame = animation[animIndex];
	   }
   }
}

void AnimObject::doUpdate(DirtySet& dirtyset, float delta)
{
   Object::doUpdate(dirtyset, delta);

   animate(delta);

   if ( _animChanged )
   {
      _animChanged = false;
      setDirty();
   }
}

/*!
    \fn AnimObject::doDraw()
 */
void AnimObject::doDraw()
{
   animate(SDL_GetTicks());

	texture->enable();
	glColor3f (1,1,1);
   glPushMatrix();

   // (3D only: negate the y coord)
   //glTranslatef(pos.x, pos.y, 0);
   glRotatef(angle, 0,0,1);
	glBegin (GL_QUADS);

   const Vector& texcoord = _textureCoords[animFrame];

   if (dir)
   {
      glMultiTexCoord2f (GL_TEXTURE0, texcoord.x, texcoord.y);
      glVertex2f (-halfX,-halfY);

      glMultiTexCoord2f (GL_TEXTURE0, texcoord.x, texcoord.y + _textureCoords.getFrameHeight());
      glVertex2f (-halfX,halfY);

      glMultiTexCoord2f (GL_TEXTURE0, texcoord.x + _textureCoords.getFrameWidth(), texcoord.y + _textureCoords.getFrameHeight());
      glVertex2f (halfX,halfY);

		glMultiTexCoord2f (GL_TEXTURE0, texcoord.x + _textureCoords.getFrameWidth(), texcoord.y);
      glVertex2f (halfX,-halfY);
	}
	else
   {
		glMultiTexCoord2f (GL_TEXTURE0, texcoord.x + _textureCoords.getFrameWidth(), texcoord.y);
		glVertex2f (-halfX,-halfY);

		glMultiTexCoord2f (GL_TEXTURE0, texcoord.x + _textureCoords.getFrameWidth(), texcoord.y + _textureCoords.getFrameHeight());
		glVertex2f (-halfX,halfY);

		glMultiTexCoord2f (GL_TEXTURE0, texcoord.x, texcoord.y + _textureCoords.getFrameHeight());
		glVertex2f (halfX,halfY);

		glMultiTexCoord2f (GL_TEXTURE0, texcoord.x, texcoord.y);
		glVertex2f (halfX,-halfY);
	}

	glEnd();
   texture->disable ();

   glPopMatrix();
}

AnimObject* AnimObject::clone ()
{
	AnimObject* c = new AnimObject();
	c->_animations = _animations;
	c->animSpeed = animSpeed;
	c->visible = visible;
	c->moveSpeed = moveSpeed;
	c->texture = texture;
	c->width = width;
	c->height = height;
	c->radius = radius;
	return c;
}

//////////////////////////////////////////////////////////////////////////
// - Visitor interface
//////////////////////////////////////////////////////////////////////////

void AnimObject::accept(NodeVisitor& nv)
{
   nv.visitAnimObject(this);
}

//////////////////////////////////////////////////////////////////////////
// - Packing & Unpacking
//////////////////////////////////////////////////////////////////////////

void AnimObject::pack(BitStream& stream) const
{
   Object::pack(stream);

   stream << animCur;
}

void AnimObject::unpack(BitStream& stream)
{
   Object::unpack(stream);

   int currentAnimation = 0;
   stream >> currentAnimation;
   setAnimation(currentAnimation);
}
