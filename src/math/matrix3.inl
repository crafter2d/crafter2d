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
#include "../defines.h"

inline Matrix3::Matrix3():
   mData()
{
   mData[1] = mData[2] = 0;
   mData[3] = mData[5] = 0;
   mData[6] = mData[7] = 0;

   mData[0] = mData[4] = mData[8] = 1;
}

inline Matrix3::~Matrix3()
{
}

inline float& Matrix3::operator[](int index)
{
   ASSERT(index >= 0 && index < 9);
   return mData[index];
}

inline float Matrix3::operator[](int index) const
{
   ASSERT(index >= 0 && index < 9);
   return mData[index];
}
