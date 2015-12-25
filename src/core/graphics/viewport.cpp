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
#include "viewport.h"
#ifndef JENGINE_INLINE
#  include "viewport.inl"
#endif

namespace Graphics
{

Viewport::Viewport():
   mLeft(0),
   mTop(0),
   mWidth(0),
   mHeight(0)
{
}

Viewport::Viewport(int left, int top, int width, int height):
   mLeft(left),
   mTop(top),
   mWidth(width),
   mHeight(height)
{
}

Viewport::~Viewport()
{
}

Viewport& Viewport::operator=(const Viewport& that)
{
   mLeft = that.mLeft;
   mTop = that.mTop;
   mWidth = that.mWidth;
   mHeight = that.mHeight;
   return *this;
}

};
