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
#include "physicsbody.h"
#ifndef JENGINE_INLINE
#  include "physicsbody.inl"
#endif

#include "physicsxml.h"

PhysicsBody::PhysicsBody():
   Body(),
   mGenerators(),
   mLinearVelocity(),
   mAngularVelocity(0.0f),
   mAccumForce(),
   mAccumTorque(0.0f),
   mLinearDamping(0.0f),
   mAngularDamping(0.0f),
   mInverseInertia(0.0f),
   mInverseMass(0.0f)
{
}

PhysicsBody::~PhysicsBody()
{
}

// ----------------------------------
// - Loading
// ----------------------------------

void PhysicsBody::load(const TiXmlElement& element)
{
   Body::load(element);

   PhysicsXML::parseXML(*this, element);
}

// ----------------------------------
// -- Forces
// ----------------------------------

void PhysicsBody::addForce(const Vector& force, const Vector& point)
{
   Vector pt = localToWorld(point);
   addWorldForce(force, pt);
}

void PhysicsBody::addWorldForce(const Vector& force, const Vector& location)
{
   Vector pt = location - getPosition();

   mAccumForce += force;
   mAccumTorque += pt.cross(force);
}

// ----------------------------------
// -- Force Generators
// ----------------------------------

void PhysicsBody::addForceGenerator(ForceGenerator* pgenerator)
{
   mGenerators.push_back(pgenerator);
}

// ----------------------------------
// -- Integration
// ----------------------------------

void PhysicsBody::integrate(float timestep)
{
   mGenerators.applyForces(*this);

   mLinearVelocity  += timestep * (mAccumForce * mInverseMass);
   mAngularVelocity += timestep * (mAccumTorque * mInverseInertia);

   mLinearVelocity  *= powf(mLinearDamping, timestep);
   mAngularVelocity *= powf(mAngularDamping, timestep);

   mPosition += mLinearVelocity;
   mAngle    += mAngularVelocity;

   clearAccumulates();

   Body::integrate(timestep);
}

void PhysicsBody::clearAccumulates()
{
   mAccumForce  = Vector::zero();
   mAccumTorque = 0.0f;
}
