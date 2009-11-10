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
#include "matrix2x2.h"

#include <math.h>

Matrix2x2::Matrix2x2():
   m11(0.0f),
   m12(0.0f),
   m21(0.0f),
   m22(0.0f)
{
}

Matrix2x2::~Matrix2x2()
{
}

void Matrix2x2::setRotation(float deg)
{
   float cos = cosf(deg);
   float sin = sinf(deg);

   m11 = cos;
   m12 = -sin;
   m21 = sin;
   m22 = cos;
}

void Matrix::setScale(float x, float y)
{
   m11 = x;
   m12 = 0;
   m21 = 0;
   m22 = y;
}

Vector Matrix::transform(const Vector& point) const
{
   return Vector(point.x * m11 + point.y * m21,
                 point.x * m12 + point.y * m22);
}
