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
#include "collisionplane.h"

#include <memory>

CollisionPlane* CollisionPlane::construct(const Vector& left, const Vector& right)
{
   Vector diff = right - left;

   std::unique_ptr<CollisionPlane> plane(new CollisionPlane());
   plane->mNormal.set(-diff.y, diff.x);
   plane->mNormal.normalize();
   plane->mLeft = left;
   plane->mRight = right;
   plane->mOffset = plane->mNormal.dot(left);

   return plane.release();
}

CollisionPlane::CollisionPlane():
   CollisionShape(CollisionShape::ePlane),
   mNormal(),
   mOffset(0.0f),
   mLeft(),
   mRight()
{
}

CollisionPlane::~CollisionPlane()
{
}

const Vector& CollisionPlane::getNormal() const
{
   return mNormal;
}

float CollisionPlane::getOffset() const
{
   return mOffset;
}

bool CollisionPlane::hitTest(const Vector& point) const
{
   float linelength = mLeft.distance(mRight);

   float u = ( ( ( point.x - mLeft.x ) * ( mRight.x - mLeft.x ) ) +
               ( ( point.y - mLeft.y ) * ( mRight.y - mLeft.y ) ) ) /
            ( linelength * linelength );

   if ( u < -0.001f || u > 1.001f )
     return false;

   return true;
}
