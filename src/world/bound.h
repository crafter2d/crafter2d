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
#ifndef _BOUND_H_
#define _BOUND_H_

#include "../math/vector.h"

/**
@author Jeroen Broekhuizen
\brief Bound for collision detection, can also be used to represent lines.

To do exact collision detection, JEngine implements bounds as non-passable (and invisible) lines in the
world to which object can collide during for example movement or grafity. Using line math an exact location
of intersection can be determined.
*/
class Bound
{
public:
   enum CollisionType { eCoinsident, eCollision, eParellel, eNoCollision };

   Bound();
   Bound(const Vector& p3, const Vector& p4);

 // get/set interface
   const Vector&  getLeft() const;
   void           setLeft(const Vector& left);

   const Vector&  getRight() const;
   void           setRight(const Vector& right);

 //operations
	void           set (int bounds[4]);
   bool           hitTest(const Vector& point, float& distance);
   CollisionType  intersect (const Vector& p3, const Vector& p4, Vector& ip);
	Vector         getNormal ();

   void     move(const Vector& offset);

   int      findPoint(const Vector& point);
   void     movePoint(int point, const Vector& offset);

private:
   void  calculateNormal();

	Vector _p1;
   Vector _p2;
	Vector _normal;
};

#ifdef JENGINE_INLINE
#  include "bound.inl"
#endif

#endif
