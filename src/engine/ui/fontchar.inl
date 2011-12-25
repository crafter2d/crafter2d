/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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

// - Get/set

INLINE int FontChar::getLeft() const
{
   return mLeft;
}

INLINE int FontChar::getTop() const
{
   return mTop;
}

INLINE int FontChar::getWidth() const
{
   return mWidth;
}

INLINE int FontChar::getHeight() const
{
   return mHeight;
}

INLINE int FontChar::getAdvanceX() const
{
   return mAdvanceX;
}

INLINE int FontChar::getAdvanceY() const
{
   return mAdvanceY;
}