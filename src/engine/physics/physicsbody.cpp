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
#include "collisionshape.h"

PhysicsBody::PhysicsBody(Simulator& simulator, Actor& actor):
   Body(simulator, actor),
   mTransform(),
   mShapes(),
   mLinearVelocity(),
   mAngularVelocity(0.0f),
   mAccumForce(),
   mAccumTorque(0.0f),
   mAcceleration(),
   mLastFrameAcceleration(),
   mPosition(),
   mAngle(0.0f),
   mLinearDamping(0.95f),
   mAngularDamping(1.0f),
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
// -- Space conversion
// ----------------------------------

Vector PhysicsBody::localToWorld(const Vector& vector) const
{
   //return mTransform.transform(vector);
   return vector;
}

// ----------------------------------
// -- Shapes
// ----------------------------------

void PhysicsBody::addShape(CollisionShape* pshape)
{
   pshape->setBody(*this);

   mShapes.push_back(pshape);
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
// -- Integration
// ----------------------------------

void PhysicsBody::integrate(float timestep)
{
   getForceGenerators().applyForces(*this);

   mLastFrameAcceleration = mAcceleration + (mAccumForce * mInverseMass);

   mLinearVelocity  += timestep * mLastFrameAcceleration;
   mAngularVelocity += timestep * (mAccumTorque * mInverseInertia);

   mLinearVelocity  *= powf(mLinearDamping, timestep);
   mAngularVelocity *= powf(mAngularDamping, timestep);

   mPosition += mLinearVelocity * timestep;
   mAngle    += mAngularVelocity * timestep;

   calculateDerivedData();

   clearAccumulates();
}

void PhysicsBody::finalize()
{
}

void PhysicsBody::clearAccumulates()
{
   mAccumForce  = Vector::zero();
   mAccumTorque = 0.0f;
}

void PhysicsBody::calculateDerivedData()
{
   mTransform.set(mPosition, mAngle);
}
