/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "bound.h"
#ifndef JENGINE_INLINE
#  include "bound.inl"
#endif

Bound::Bound():
   mLeft(),
   mRight(),
   mNormal()
{
}

Bound::Bound(const Vector& left, const Vector& right):
   mLeft(left),
   mRight(right),
   mNormal()
{
   calculateNormal();
}

void Bound::calculateNormal()
{
   Vector direction = mRight - mLeft;
   mNormal.set(-direction.y, direction.x);
   mNormal.normalize();
}

bool Bound::hitTest(const Vector& point, float& distance)
{
   float linelength = mLeft.distance(mRight);

   float u = ( ( ( point.x - mLeft.x ) * ( mRight.x - mLeft.x ) ) +
               ( ( point.y - mLeft.y ) * ( mRight.y - mLeft.y ) ) ) /
            ( linelength * linelength );

   if ( u < -0.001f || u > 1.001f )
     return false;   // closest point does not fall within the line segment

   Vector intersection = mLeft + (mRight - mLeft) * u;

   distance = point.distance(intersection);

   return true;
}

// http://astronomy.swin.edu.au/~pbourke/geometry/lineline2d/, 2005
/// \fn Bound::intersect(const Vector& p3, const Vector& p4, Vector& ip)
/// \brief To calculate the intersection point between a line and this bound call this
/// function. It quickly calculates the exact location on the line where the intersection
/// took place.
/// \param [in] p3 start of the line
/// \param [in] p4 end of the line
/// \param [out] ip intersection point (valid if function returns true)
/// \retval true there was an intersection with this bound
/// \retval false no intersection was detected
Bound::CollisionType Bound::intersect(const Vector& p3, const Vector& p4, Vector& ip)
{
	// calculate the denominator
	float denom = ((p4.y - p3.y) * (mRight.x - mLeft.x)) - ((p4.x - p3.x) * (mRight.y - mLeft.y));

   float nume_a = (( p4.x - p3.x)  * (mLeft.y-p3.y)) - ((p4.y - p3.y) * (mLeft.x-p3.x));
   float nume_b = ((mRight.x - mLeft.x) * (mLeft.y-p3.y)) - ((mRight.y - mLeft.y) * (mLeft.x-p3.x));

	if ( denom == 0 )
   {
      if( nume_a == 0.0f && nume_b == 0.0f )
      {
         return eCoinsident;
      }

		return eParellel;
   }

   float ua = nume_a / denom;
	float ub = nume_b / denom;

	if ( ua > -0.001f && ua < 1.001f && ub > -0.001f && ub < 1.001f )
   {
		ip = mLeft + (mRight - mLeft) * ua;
		return eCollision;
	}

   return eNoCollision;
}

//////////////////////////////////////////////////////////////////////////
// - Editing interface
//////////////////////////////////////////////////////////////////////////

void Bound::move(const Vector& offset)
{
   mLeft += offset;
   mRight += offset;
}

int Bound::findPoint(const Vector& point)
{
   if ( (point - mLeft).length() < 4 )
   {
      return 0;
   }
   else if ( (point - mRight).length() < 4 )
   {
      return 1;
   }

   return -1;
}

void Bound::movePoint(int point, const Vector& offset)
{
   if ( point == 0 )
   {
      mLeft += offset;
   }
   else if ( point == 1 )
   {
      mRight += offset;
   }

   calculateNormal();
}
