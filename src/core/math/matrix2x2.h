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
#ifndef MATRIX_2x2_H_
#define MATRIX_2x2_H_

#include "core/core_base.h"

#include "vector.h"

class CORE_API Matrix2x2
{
public:
   Matrix2x2();
   ~Matrix2x2();

   void setComponents(const Vector& one, const Vector& two);
   void setRotation(float deg);
   void setScale(float x, float y);

   Vector transform(const Vector& point) const;
   Vector transformTranspose(const Vector& point) const;

private:
   float m11, m12;
   float m21, m22;
};

#endif
