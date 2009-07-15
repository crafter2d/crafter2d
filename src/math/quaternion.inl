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
#include "..\defines.h"

INLINE float Quaternion::r() const
{
   return mData[0];
}

INLINE void Quaternion::r(float nr)
{
   mData[0] = nr;
}

INLINE float Quaternion::i() const
{
   return mData[1];
}

INLINE void Quaternion::i(float ni)
{
   mData[1] = ni;
}

INLINE float Quaternion::j() const
{
   return mData[2];
}

INLINE void Quaternion::j(float nj)
{
   mData[2] = nj;
}

INLINE float Quaternion::k() const
{
   return mData[3];
}

INLINE void Quaternion::k(float nk)
{
   mData[3] = nk;
}
