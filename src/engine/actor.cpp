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

#include "core/log/log.h"
#include "core/graphics/rendercontext.h"

#include "engine/components/meshcomponent.h"
#include "engine/components/componentstructs.h"
#include "engine/components/componentmessage.h"
#include "engine/net/netstream.h"

#include "animator.h"
#include "nodevisitor.h"
#include "controller.h"

using namespace Graphics;

IMPLEMENT_REPLICATABLE(ActorId, Actor, Entity)

Actor::Actor():
   Entity(),
   mpController(NULL),
   mWidth(0),
   mHeight(0),
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
   ASSERT(hasMesh());
   getMesh().render(context);
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
   if ( hasMesh() )
      return getMesh().getPosition();
   else
   {
      PositionInfo info;
      const_cast<Actor&>(*this).sendComponentMessage(ComponentMessage(ComponentInterface::eQueryPositionMsg, &info));
      return info.transform.getPosition();
   }
}

/// \fn Actor::setPosition(const Vector& p)
/// \brief Set the position of the object in world coordinates.
/// \param p the new position of the object
void Actor::setPosition(const Vector& p)
{
   PositionInfo info;
   sendComponentMessage(ComponentMessage(ComponentInterface::eQueryPositionMsg, &info));
   
   info.transform.setPosition(p);

   sendComponentMessage(ComponentMessage(ComponentInterface::ePositionMsg, &info));

   setDirty(ePositionDirty);
}

/// \fn Actor::getRotation() const
/// \brief Returns the rotation in degrees of the object.
float Actor::getRotation() const
{
   if ( hasMesh() )
      return getMesh().getAngle();
   else
   {
      PositionInfo info;
      const_cast<Actor&>(*this).sendComponentMessage(ComponentMessage(ComponentInterface::eQueryPositionMsg, &info));
      return info.transform.getAngle();
   }
}

/// \fn void setRotation(const float deg)
/// \brief Sets the new degree of rotation of this object.
/// \param deg the new rotation degree
void Actor::setRotation(const float deg) 
{
   PositionInfo info;
   sendComponentMessage(ComponentMessage(ComponentInterface::eQueryPositionMsg, &info));
   
   info.transform.setAngle(deg);

   sendComponentMessage(ComponentMessage(ComponentInterface::ePositionMsg, &info));

   setDirty(ePositionDirty);
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
      PositionInfo info;
      const_cast<Actor&>(*this).sendComponentMessage(ComponentMessage(ComponentInterface::eQueryPositionMsg, &info));

      stream << info.transform.getPosition().x << info.transform.getPosition().y << info.transform.getAngle() << dir;
   }
}

void Actor::doUnpack(DataStream& stream)
{
   Entity::doUnpack(stream);

   if ( isDirty(ePositionDirty) )
   {
      Vector pos;
      float angle;

      stream >> pos.x >> pos.y >> angle >> dir;

      PositionInfo info;
      info.transform.set(pos, angle);
      sendComponentMessage(ComponentMessage(ComponentInterface::ePositionMsg, &info));

      setDirty(ePositionDirty);
   }
}
