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
#ifndef BODY_H_
#define BODY_H_

#include "..\math\xform.h"

class Body
{
public:
   Body();
   ~Body();

   float getMass() const;
   void  setMass(float mass);

   void  addForce(const Vector& force);
   void  addLocalizedForce(const Vector& force, const Vector& location);

private:
   Vector localToWorld(const Vector& vector) const;

   XForm    mTransform;
   Vector   mPosition;
   float    mOrientation;
   Vector   mAccumForce;
   Vector   mAccumTorque;
   float    mInverseMass;
};

#ifdef JENGINE_INLINE
#  include "body.inl"
#endif

#endif
