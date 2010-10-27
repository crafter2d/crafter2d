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
#include "core/defines.h"

INLINE float PhysicsBody::getInverseMass() const
{
   return mInverseMass;
}

INLINE float PhysicsBody::getMass() const
{
   return 1.0f / mInverseMass;
}

INLINE void PhysicsBody::setMass(float mass)
{
   mInverseMass = 1.0f / mass;
}

INLINE float PhysicsBody::getInverseInertia() const
{
   return mInverseInertia;
}

INLINE float PhysicsBody::getInertia() const
{
   return 1.0f / mInverseInertia;
}

INLINE void PhysicsBody::setInertia(float inertia)
{
   mInverseInertia = 1.0f / inertia;
}

INLINE const Vector& PhysicsBody::getVelocity() const
{
   return mLinearVelocity;
}

INLINE const Vector& PhysicsBody::getAcceleration() const
{
   return mAcceleration;
}

INLINE void PhysicsBody::setAcceleration(const Vector& acc)
{
   mAcceleration = acc;
}

INLINE const Vector& PhysicsBody::getLastFrameAcceleration() const
{
   return mLastFrameAcceleration;
}

// Operations

INLINE void PhysicsBody::addForce(const Vector& force)
{
   mAccumForce += force;
}

INLINE void PhysicsBody::addTorque(float torque)
{
   mAccumTorque += torque;
}
