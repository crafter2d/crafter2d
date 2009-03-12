/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
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
   _p1(),
   _p2(),
   _normal()
{
}

Bound::Bound(const Vector& _p1, const Vector& _p2):
   _p1(_p1),
   _p2(_p2),
   _normal()
{
   calculateNormal();
}

/// \fn Bound::set(int bounds[4])
/// \brief Initialize this bound with its line data and precalculates the normal of this bound.
void Bound::set (int bounds[4])
{
	_p1.x = bounds[0]; _p1.y = bounds[1];
	_p2.x = bounds[2]; _p2.y = bounds[3];

	calculateNormal();
}

void Bound::calculateNormal()
{
   Vector direction = _p2 - _p1;
   _normal.set(-direction.y, direction.x);
   _normal.normalize();
}

float magnitude(const Vector& v1, const Vector& v2)
{
   return (v2 - v1).length();
}

bool Bound::hitTest(const Vector& point, float& distance)
{
   float linelength = magnitude(_p1, _p2);

   float u = ( ( ( point.x - _p1.x ) * ( _p2.x - _p1.x ) ) +
               ( ( point.y - _p1.y ) * ( _p2.y - _p1.y ) ) ) /
            ( linelength * linelength );

   if ( u < 0.0f || u > 1.0f )
     return false;   // closest point does not fall within the line segment

   Vector intersection = _p1 + (_p2 - _p1) * u;
   //intersection.x = _p1.x + U * ( _p2.x - _p1.x );
   //intersection.y = _p1.y + U * ( _p2.y - _p1.y );

   distance = magnitude(point, intersection);

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
bool Bound::intersect (const Vector& p3, const Vector& p4, Vector& ip)
{
	// calculate the denominator
	float denom = float((p4.y-p3.y)*(_p2.x-_p1.x)) - ((p4.x-p3.x)*(_p2.y-_p1.y));
	if (denom == 0)
		return false;

	float x1 = _p1.y-p3.y;
	float x2 = _p1.x-p3.x;

	float ua = (((p4.x-p3.x)*x1) - ((p4.y-p3.y)*x2)) / denom;
	float ub = (((_p2.x-_p1.x)*x1) - ((_p2.y-_p1.y)*x2)) / denom;

	if (ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f) {
		ip = _p1 + (_p2 - _p1) * ua;
		return true;
	}
	else
		return false;
}

void Bound::move(const Vector& offset)
{
   _p1 += offset;
   _p2 += offset;
}

int Bound::findPoint(const Vector& point)
{
   if ( (point - _p1).length() < 4 )
   {
      return 0;
   }
   else if ( (point - _p2).length() < 4 )
   {
      return 1;
   }

   return -1;
}

void Bound::movePoint(int point, const Vector& offset)
{
   if ( point == 0 )
   {
      _p1 += offset;
   }
   else if ( point == 1 )
   {
      _p2 += offset;
   }

   calculateNormal();
}
