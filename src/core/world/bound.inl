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
#include "core/defines.h"

/// \fn Bound::getNormal()
/// \brief Use this function to get the normal of this bound. The normal can be used for collision
/// detection etc.
/// \returns the normal of this bound
INLINE const Vector& Bound::getNormal()
{
	return mNormal;
}

INLINE const Vector& Bound::getLeft() const
{
   return mLeft;
}

INLINE void Bound::setLeft(const Vector& left)
{
   mLeft = left;
}

INLINE const Vector& Bound::getRight() const
{
   return mRight;
}

INLINE void Bound::setRight(const Vector& right)
{
   mRight = right;
}
