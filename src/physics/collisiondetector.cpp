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
#include "collisiondetector.h"

#include "../math/vector.h"

#include "collisioncircle.h"
#include "collisionplane.h"
#include "collisiondata.h"
#include "contact.h"

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
}

bool CollisionDetector::circleAndCircle(CollisionData& data, const CollisionCircle& one, const CollisionCircle& two)
{
   const Vector& posOne = one.getPosition();
   const Vector& posTwo = two.getPosition();

   Vector diff = posOne - posTwo;
   float distance = diff.length();
   if ( distance <= 0.0f || distance > one.getRadius() + two.getRadius() )
   {
      return false;
   }

   Contact& contact     = data.getNext();
   contact.mPoint       = posOne + (diff * 0.5);
   contact.mNormal      = diff * (1.0f / distance);
   contact.mPenetration = one.getRadius() + two.getRadius() - distance;

   return true;
}

bool CollisionDetector::circleAndPlane(CollisionData& data, const CollisionCircle& circle, const CollisionPlane& plane)
{
   const Vector& position = circle.getPosition();

   float distance = plane.getNormal().dot(position)
                  - circle.getRadius()
                  - plane.getOffset();
   if ( distance >= 0.0f )
   {
      return false;
   }

   Contact& contact     = data.getNext();
   contact.mPoint       = position - plane.getNormal() * (distance + circle.getRadius());
   contact.mNormal      = plane.getNormal();
   contact.mPenetration = -distance;

   return true;
}
