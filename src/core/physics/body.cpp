/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#include "body.h"
#ifndef JENGINE_INLINE
#  include "body.inl"
#endif

#include "bodylistener.h"
#include "simulator.h"
#include "forcegenerator.h"

// ----------------------------------
// - Body
// ----------------------------------

Body::Body(Simulator& simulator):
   mSimulator(simulator),
   mTransform(),
   mpEntity(NULL),
   mpListener(NULL),
   mForceGenerators()
{
}

Body::~Body()
{
   cleanUp();
}

// ----------------------------------
// -- Query interface
// ----------------------------------

bool Body::hasLineOfSight(const Body& that) const
{
   return mSimulator.lineOfSight(*this, that);
}

// - Operations

void Body::cleanUp()
{
   for ( std::size_t index = 0; index < mForceGenerators.size(); index++ )
   {
      delete mForceGenerators[index];
   }
   mForceGenerators.clear();
}

// -- Maintenance

void Body::addListener(IBodyListener* plistener)
{
   mpListener = plistener;
}

void Body::link(Body& to, const JointDefinition& definition)
{
   mSimulator.createLink(*this, to, definition);
}

// ----------------------------------
// -- Force Generators
// ----------------------------------

void Body::addForceGenerator(ForceGenerator* pgenerator)
{
   mForceGenerators.push_back(pgenerator);
}

void Body::applyForce(const Vector& force)
{
   C2D_UNUSED(force);
}

void Body::applyForce(const Vector& force, const Vector& pos)
{
   C2D_UNUSED(force);
   C2D_UNUSED(pos);
}

void Body::applyImpulse(const Vector& impulse)
{
   C2D_UNUSED(impulse);
}

// ----------------------------------
// -- Notifications
// ----------------------------------

void Body::notifyPositionChanged()
{
}

// -- listener notifications

void Body::firePositionChanged()
{
   if ( mpListener != NULL )
   {
      mpListener->onPositionChanged(*this);
   }
}

// ----------------------------------
// -- Integration
// ----------------------------------

void Body::finalize()
{
}
