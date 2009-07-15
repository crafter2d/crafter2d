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
#ifndef RIGID_BODY_H_
#define RIGID_BODY_H_

#include "../math/matrix3.h"
#include "../math/matrix4.h"
#include "../math/quaternion.h"
#include "../math/vector3.h"

class RigidBody
{
public:
   RigidBody();
   ~RigidBody();

   void setInertiaTensor(const Matrix4& tensor);

   void calculateDerivedData();

private:
   void calculateTransformMatrix(Matrix4& transform, const Vector3& position, const Quaternion& orientation);
   void transformInertiaTensor(Matrix3& worldtensor, const Matrix4& inertiatensor, const Matrix4& tranform, const Quaternion& orientation);

   Matrix4     mTransformation;
   Matrix4     mInverseInertiaTensor;
   Matrix3     mInverseInertiaTensorWorld;
   Vector3     mPosition;
   Vector3     mVelocity;
   Vector3     mRotation;
   Quaternion  mOrientation;
   float       mInverseMass;
};

#endif
