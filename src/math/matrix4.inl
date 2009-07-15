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

inline Matrix4::Matrix4():
   mData()
{
   mData[1] = mData[2] = mData[3] = 0;
   mData[4] = mData[6] = mData[7] = 0;
   mData[8] = mData[9] = mData[11] = 0;

   mData[0] = mData[5] = mData[10] = 1;
}

inline Matrix4::~Matrix4()
{
}

inline float& Matrix4::operator[](int index)
{
   ASSERT(index >= 0 && index < 12)
   return mData[index];
}

inline float Matrix4::operator[](int index) const
{
   ASSERT(index >= 0 && index < 12)
   return mData[index];
}

inline void Matrix4::set(int index, float value)
{
   ASSERT(index >= 0 && index < 12)
   mData[index] = value;
}