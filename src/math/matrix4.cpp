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
#include "matrix4.h"

float Matrix4::getDeterminant() const
{
   return mData[8] * mData[5] * mData[2] +
          mData[4] * mData[9] * mData[2] +
          mData[8] * mData[1] * mData[6] -
          mData[0] * mData[9] * mData[6] -
          mData[4] * mData[1] * mData[10] +
          mData[0] * mData[5] * mData[10];
}

void Matrix4::setInverse(const Matrix4& m)
{
   float det = m.getDeterminant();
   if ( det == 0.0f )
      return;

   det = 1.0f / det;

   mData[0]  = (-m[9] * m[6] + m[5] * m[10]) * det;
   mData[4]  = ( m[8] * m[6] - m[4] * m[10]) * det;
   mData[8]  = (-m[8] * m[5] + m[4] * m[9] ) * det;

   mData[1]  = ( m[9] * m[2] - m[1] * m[10]) * det;
   mData[5]  = (-m[8] * m[2] + m[0] * m[10]) * det;
   mData[9]  = ( m[8] * m[1] - m[0] * m[9] ) * det;

   mData[2]  = (-m[5] * m[2] + m[1] * m[6] ) * det;
   mData[6]  = ( m[4] * m[2] - m[0] * m[6] ) * det;
   mData[10] = (-m[4] * m[1] + m[0] * m[5] ) * det;

   mData[3]  = (m[9] * m[6]  * m[3]
             -  m[5] * m[10] * m[3]
             -  m[9] * m[2]  * m[7]
             +  m[1] * m[10] * m[7]
             +  m[5] * m[2]  * m[11]
             -  m[1] * m[6]  * m[11]) * det;

   mData[7]  = (-m[8] * m[6]  * m[3]
             +   m[4] * m[10] * m[3]
             +   m[8] * m[2]  * m[7]
             -   m[0] * m[10] * m[7]
             -   m[4] * m[2]  * m[11]
             +   m[0] * m[6]  * m[11]) * det;

   mData[11] = (m[8] * m[5] * m[3]
             -  m[4] * m[9] * m[3]
             -  m[8] * m[1] * m[7]
             +  m[0] * m[9] * m[7]
             +  m[4] * m[1] * m[11]
             -  m[0] * m[5] * m[11]) * det;
}
