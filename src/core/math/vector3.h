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
#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "core/core_base.h"

class CORE_API Vector3
{
public:
   Vector3();
   Vector3(const Vector3& that);
   Vector3(float x, float y, float z);
   ~Vector3();

 // get/set
   float x() const;
   void  x(float nx);

   float y() const;
   void  y(float ny);

   float z() const;
   void  z(float nz);

   void set(float x, float y, float z);

private:
   float mX;
   float mY;
   float mZ;
   float mPad;
};

#include "vector3.inl"

#endif
