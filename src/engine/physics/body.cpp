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

#include <tinyxml.h>

#include "bodylistener.h"
#include "collisionshape.h"
#include "simulator.h"
#include "forcegenerator.h"

// ----------------------------------
// - Static
// ----------------------------------

bool Body::hasInfo(const TiXmlElement& element)
{
   return element.FirstChildElement("body") != NULL;
}

// ----------------------------------
// - Body
// ----------------------------------

Body::Body(Simulator& simulator):
   mSimulator(simulator),
   mTransform(),
   mForceGenerators()
{
}

Body::~Body()
{
   cleanUp();
}

// ----------------------------------
// - Loading
// ----------------------------------

void Body::load(const TiXmlElement& element)
{
   // nothing to be done
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

// ----------------------------------
// -- Force Generators
// ----------------------------------

void Body::addForceGenerator(ForceGenerator* pgenerator)
{
   mForceGenerators.push_back(pgenerator);
}

void Body::applyForce(const Vector& force)
{
}

void Body::applyForce(const Vector& force, const Vector& pos)
{
}

void Body::applyImpulse(const Vector& impulse)
{
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

void Body::integrate(float timestep)
{
   PURE_VIRTUAL
}

void Body::finalize()
{
}
