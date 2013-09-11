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
   if ( mpController != NULL )
      mpController->performAction(*this);
}

void Actor::doUpdateClient(float delta)
{
}

/*!
    \fn Actor::draw()
	 \brief Draws the object on the screen.
 */
void Actor::doDraw(RenderContext& context) const
{
}

/// \fn Actor::clone ()
/// \brief Currently not implemented yet!
/// \returns always NULL
Actor* Actor::clone ()
{
	return NULL;
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
