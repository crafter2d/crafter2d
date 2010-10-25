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

inline Vector3::Vector3():
   mX(0),
   mY(0),
   mZ(0),
   mPad(0)
{
}

inline Vector3::Vector3(const Vector3& that):
   mX(that.mX),
   mY(that.mY),
   mZ(that.mZ),
   mPad(0)
{
}

inline Vector3::~Vector3()
{
}

// get/set
inline float Vector3::x() const
{
   return mX;
}

inline void Vector3::x(float nx)
{
   mX = nx;
}

inline float Vector3::y() const
{
   return mY;
}

inline void Vector3::y(float ny)
{
   mY = ny;
}

inline float Vector3::z() const
{
   return mZ;
}

inline void Vector3::z(float nz)
{
   mZ = nz;
}

// operations

inline void Vector3::set(float x, float y, float z)
{
   mX = x;
   mY = y;
   mZ = z;
}
