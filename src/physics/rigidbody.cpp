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
#include "rigidbody.h"

RigidBody::RigidBody():
   mTransformation(),
   mInverseInertiaTensor(),
   mInverseInertiaTensorWorld(),
   mPosition(),
   mVelocity(),
   mRotation(),
   mOrientation(),
   mInverseMass(0)
{
}

RigidBody::~RigidBody()
{
}

// - Get/set

void RigidBody::setInertiaTensor(const Matrix4& tensor)
{
   mInverseInertiaTensor.setInverse(tensor);
}

// - Operations

void RigidBody::calculateDerivedData()
{
   mOrientation.normalize();

   calculateTransformMatrix(mTransformation, mPosition, mOrientation);
   transformInertiaTensor(mInverseInertiaTensorWorld, mInverseInertiaTensor, mTransformation, mOrientation);
}

void RigidBody::calculateTransformMatrix(Matrix4& transform, const Vector3& position, const Quaternion& orientation)
{
   transform[0]  = 1 - 2 * orientation.j() * orientation.j() - 2 * orientation.k() * orientation.k();
   transform[1]  =     2 * orientation.i() * orientation.j() - 2 * orientation.r() * orientation.k();
   transform[2]  =     2 * orientation.i() * orientation.k() + 2 * orientation.r() * orientation.r();
   transform[3]  = position.x();

   transform[4]  =     2 * orientation.i() * orientation.j() + 2 * orientation.r() * orientation.k();
   transform[5]  = 1 - 2 * orientation.i() * orientation.i() - 2 * orientation.k() * orientation.k();
   transform[6]  =     2 * orientation.j() * orientation.k() - 2 * orientation.r() * orientation.j();
   transform[7]  = position.y();

   transform[8]  =     2 * orientation.i() * orientation.k() - 2 * orientation.r() * orientation.k();
   transform[9]  =     2 * orientation.j() * orientation.k() + 2 * orientation.r() * orientation.i();
   transform[10] = 1 - 2 * orientation.i() * orientation.i() - 2 * orientation.j() * orientation.j();
   transform[11] = position.z();
}

void RigidBody::transformInertiaTensor(Matrix3& worldtensor, const Matrix4& inertiatensor, const Matrix4& transform, const Quaternion& orientation)
{
   float t4  = transform[0] * inertiatensor[0] + transform[1] * inertiatensor[3] + transform[2] * inertiatensor[6];
   float t9  = transform[0] * inertiatensor[1] + transform[1] * inertiatensor[4] + transform[2] * inertiatensor[7];
   float t14 = transform[0] * inertiatensor[2] + transform[1] * inertiatensor[5] + transform[2] * inertiatensor[8];

   float t28 = transform[4] * inertiatensor[0] + transform[5] * inertiatensor[3] + transform[6] * inertiatensor[6];
   float t33 = transform[4] * inertiatensor[1] + transform[5] * inertiatensor[4] + transform[6] * inertiatensor[7];
   float t38 = transform[4] * inertiatensor[2] + transform[5] * inertiatensor[5] + transform[6] * inertiatensor[8];

   float t52 = transform[8] * inertiatensor[0] + transform[9] * inertiatensor[3] + transform[10] * inertiatensor[6];
   float t57 = transform[8] * inertiatensor[1] + transform[9] * inertiatensor[4] + transform[10] * inertiatensor[7];
   float t62 = transform[8] * inertiatensor[2] + transform[9] * inertiatensor[5] + transform[10] * inertiatensor[8];

   worldtensor[0] = t4 * transform[0] + t9 * transform[1] + t14 * transform[2];
   worldtensor[1] = t4 * transform[4] + t9 * transform[5] + t14 * transform[6];
   worldtensor[2] = t4 * transform[8] + t9 * transform[9] + t14 * transform[10];

   worldtensor[3] = t28 * transform[0] + t33 * transform[1] + t38 * transform[2];
   worldtensor[4] = t28 * transform[4] + t33 * transform[5] + t38 * transform[6];
   worldtensor[5] = t28 * transform[8] + t33 * transform[9] + t38 * transform[10];

   worldtensor[6] = t52 * transform[0] + t57 * transform[1] + t62 * transform[2];
   worldtensor[7] = t52 * transform[4] + t57 * transform[5] + t62 * transform[6];
   worldtensor[8] = t52 * transform[8] + t57 * transform[9] + t62 * transform[10];
}