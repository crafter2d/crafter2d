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
#include "../defines.h"

INLINE const Vector& Body::getPosition() const
{
   return mPosition;
}

INLINE float Body::getMass() const
{
   return 1.0f / mInverseMass;
}

INLINE void Body::setMass(float mass)
{
   mInverseMass = 1.0f / mass;
}

INLINE float Body::getInertia() const
{
   return 1.0f / mInverseInertia;
}

INLINE void  Body::setInertia(float inertia)
{
}

INLINE void Body::addForce(const Vector& force)
{
   mAccumForce += force;
}

INLINE void Body::addTorque(float torque)
{
   mAccumTorque += torque;
}
