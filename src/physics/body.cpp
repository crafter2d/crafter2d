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

#include "collisionshape.h"

Body::Body():
   mTransform(),
   mPosition(),
   mAngle(0.0f),
   mLinearVelocity(),
   mAngularVelocity(0.0f),
   mAccumForce(),
   mAccumTorque(0.0f),
   mLinearDamping(0.0f),
   mAngularDamping(0.0f),
   mInverseInertia(0.0f),
   mInverseMass(0.0f),
   mpShape(NULL)
{
}

Body::~Body()
{
}

// ----------------------------------
// -- Shapes
// ----------------------------------

void Body::addShape(CollisionShape* pshape)
{
   if ( mpShape != NULL )
   {
      pshape->setNext(mpShape);
   }

   mpShape = pshape;
   mpShape->setBody(*this);
}

// ----------------------------------
// -- Forces
// ----------------------------------

void Body::addForce(const Vector& force, const Vector& point)
{
   Vector pt = localToWorld(point);
   addWorldForce(force, pt);
}

void Body::addWorldForce(const Vector& force, const Vector& location)
{
   Vector pt = location - mPosition;

   mAccumForce += force;
   mAccumTorque += pt.cross(force);
}

// ----------------------------------
// -- Space conversion
// ----------------------------------

Vector Body::localToWorld(const Vector& vector) const
{
   return mTransform.transform(vector);
}

// ----------------------------------
// -- Integration
// ----------------------------------

void Body::integrate(float timestep)
{
   mLinearVelocity  += timestep * (mAccumForce * mInverseMass);
   mAngularVelocity += timestep * (mAccumTorque * mInverseInertia);

   mLinearVelocity  *= powf(mLinearDamping, timestep);
   mAngularVelocity *= powf(mAngularDamping, timestep);

   mPosition += mLinearVelocity;
   mAngle    += mAngularVelocity;

   calculateDerivedData();
   clearAccumulates();
}

void Body::calculateDerivedData()
{
   mTransform.set(mPosition, mAngle);
}

void Body::clearAccumulates()
{
   mAccumForce  = Vector::zero();
   mAccumTorque = 0.0f;
}
