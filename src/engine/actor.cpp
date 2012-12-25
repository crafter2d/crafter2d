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

#include <tinyxml.h>

#include "core/log/log.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/rendercontext.h"

#include "engine/physics/body.h"
#include "engine/physics/simulator.h"
#include "engine/net/netstream.h"
#include "engine/resource/resourcemanager.h"
#include "engine/world/world.h"

#include "animator.h"
#include "process.h"
#include "nodevisitor.h"
#include "texturecoordinate.h"
#include "controller.h"
#include "scopedtransform.h"

using namespace Graphics;

IMPLEMENT_REPLICATABLE(ActorId, Actor, Entity)

Actor::Actor():
   Entity(),
   mpController(NULL),
   mWidth(0),
   mHeight(0),
   halfX(.0f),
   halfY(.0f),
   angle(.0f),
   visible(true),
   dir(true)
{
}

Actor::~Actor()
{
}

// - Operations

void Actor::destroy()
{
   Entity::destroy();
}

// - Query interface

bool Actor::hasLineOfSight(const Actor& that) const
{
   return false; // getBody().hasLineOfSight(that.getBody());
}

// - Modifier interface

void Actor::setController(Controller* pcontroller)
{
   mpController = pcontroller;
}

// - Updating

void Actor::doUpdate(float delta)
{
   Entity::doUpdate(delta);

   if ( mpController != NULL )
      mpController->performAction(*this);
}

void Actor::doUpdateClient(float delta)
{
   Entity::doUpdate(delta);
}

/*!
    \fn Actor::draw()
	 \brief Draws the object on the screen.
 */
void Actor::doDraw(RenderContext& context) const
{
   Entity::doDraw(context);
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
   return 0;// mpAnimator != NULL ? mpAnimator->getAnimation() : 0;
}

void Actor::setAnimation(int anim)
{
   //if ( mpAnimator != NULL && mpAnimator->setAnimation(anim) )
   {
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
   setDirty(ePositionDirty);
}

//////////////////////////////////////////////////////////////////////////
// - Packing & unpacking
//////////////////////////////////////////////////////////////////////////

void Actor::doPack(DataStream& stream) const
{
   Entity::doPack(stream);

   if ( isDirty(ePositionDirty) )
   {
      const Vector& pos = getPosition();
      stream << pos.x << pos.y << mVel.x << mVel.y << angle << dir;
   }
}

void Actor::doUnpack(DataStream& stream)
{
   Entity::doUnpack(stream);

   if ( isDirty(ePositionDirty) )
   {
      Vector pos;
      stream >> pos.x >> pos.y >> mVel.x >> mVel.y >> angle >> dir;

      setPosition(pos);
   }
}
