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
#include "actor.h"
#ifndef JENGINE_INLINE
#  include "actor.inl"
#endif

#include <GL/GLee.h>
#include <GL/glu.h>
#include <tinyxml.h>

#include "core/log/log.h"

#include "engine/physics/body.h"
#include "engine/physics/simulator.h"
#include "engine/resource/resourcemanager.h"
#include "engine/world/world.h"

#include "animator.h"
#include "process.h"
#include "nodevisitor.h"
#include "texturecoordinate.h"
#include "controller.h"
#include "scopedtransform.h"

IMPLEMENT_REPLICATABLE(ActorId, Actor, Entity)

Actor::Actor():
   Entity(),
   texture(),
   mpBody(NULL),
   mpAnimator(NULL),
   mpController(NULL),
   mType(),
   mWidth(0),
   mHeight(0),
   halfX(.0f),
   halfY(.0f),
   angle(.0f),
   visible(true),
   dir(true),
   mStatic(false)
{
}

Actor::~Actor()
{
}

// - Operations

void Actor::destroy()
{
   delete mpBody;
   mpBody = NULL;

   delete mpAnimator;
   mpAnimator = NULL;

   Entity::destroy();
}

// - Query interface

bool Actor::hasLineOfSight(const Actor& that) const
{
   return getBody().hasLineOfSight(that.getBody());
}

// - Modifier interface

void Actor::setController(Controller* pcontroller)
{
   mpController = pcontroller;
}

// - Updating

void Actor::doUpdate(float delta)
{
   if ( mpController != NULL )
      mpController->performAction(*this);
}

void Actor::doUpdateClient(float delta)
{
   // perform client side predictions
   if ( mpAnimator != NULL )
      mpAnimator->animate(delta);
}

/*!
    \fn Actor::draw()
	 \brief Draws the object on the screen.
 */
void Actor::doDraw() const
{
   ScopedTransform transform(mPos);

	texture->enable();

   TextureCoordinate texcoord = mpAnimator->getTextureCoordinate();
   if ( !dir )
      texcoord.flipHorizontal();

   Vector tex;

   glColor3f (1,1,1);
   glPushMatrix();

   glRotatef(angle, 0,0,1);
	glBegin (GL_QUADS);
      tex = texcoord.getTopLeft();
		glMultiTexCoord2f (GL_TEXTURE0_ARB, tex.x, tex.y);
		glVertex2f (-halfX,-halfY);

      tex = texcoord.getBottomLeft();
      glMultiTexCoord2f (GL_TEXTURE0_ARB, tex.x, tex.y);
		glVertex2f (-halfX,halfY);

      tex = texcoord.getBottomRight();
      glMultiTexCoord2f (GL_TEXTURE0_ARB, tex.x, tex.y);
		glVertex2f (halfX,halfY);

      tex = texcoord.getTopRight();
      glMultiTexCoord2f (GL_TEXTURE0_ARB, tex.x, tex.y);
		glVertex2f (halfX,-halfY);
	glEnd();

   texture->disable();

   glPopMatrix();
}

/// \fn Actor::clone ()
/// \brief Currently not implemented yet!
/// \returns always NULL
Actor* Actor::clone ()
{
	return NULL;
}

/// \fn Actor::getPosition () const
/// \brief Returns the current position of the object
/// \returns current position of object
const Vector& Actor::getPosition() const
{
   return mPos;
}

/// \fn Actor::setPosition(const Vector& p)
/// \brief Set the position of the object in world coordinates.
/// \param p the new position of the object
void Actor::setPosition(const Vector& p)
{
   if ( isReplica() )
      mPos = p;
   else
   {
      if ( mpBody != NULL )
         mpBody->setPosition(p);
      else
         mPos = p;

      setDirty(ePositionDirty);
   }
}

void Actor::setSize(int width, int height)
{
   mWidth = width;
   mHeight = height;

   // set the half size as well
   halfX = width * 0.5f;
   halfY = height * 0.5f;
}

int Actor::getAnimation() const
{
   return mpAnimator != NULL ? mpAnimator->getAnimation() : 0;
}

void Actor::setAnimation(int anim)
{
   if ( mpAnimator != NULL )
   {
      mpAnimator->setAnimation(anim);

      setDirty(eAnimationDirty);
   }
}

//////////////////////////////////////////////////////////////////////////
// - Visitor interface
//////////////////////////////////////////////////////////////////////////

void Actor::accept(NodeVisitor& nv)
{
   nv.visitActor(this);
}

// simulator interface

void Actor::updateState()
{
   ASSERT_PTR(mpBody);

   mPos  = mpBody->getPosition();
   angle = mpBody->getAngle();

   setDirty(ePositionDirty);
}

//////////////////////////////////////////////////////////////////////////
// - Packing & unpacking
//////////////////////////////////////////////////////////////////////////

void Actor::doPack(BitStream& stream) const
{
   Entity::doPack(stream);

   if ( isDirty(ePositionDirty) )
   {
      stream << getPosition() << mVel << angle << dir;
   }

   if ( isDirty(eAnimationDirty) )
   {
      stream << mpAnimator->getAnimation();
   }
}

void Actor::doUnpack(BitStream& stream, int dirtyflag)
{
   Entity::doUnpack(stream, dirtyflag);

   if ( IS_SET(dirtyflag, ePositionDirty) )
   {
      Vector pos;
      stream >> pos >> mVel >> angle >> dir;

      setPosition(pos);
   }

   if ( IS_SET(dirtyflag, eAnimationDirty) )
   {
      int anim;

      stream >> anim;

      if ( mpAnimator != NULL )
         mpAnimator->setAnimation(anim);
   }
}
