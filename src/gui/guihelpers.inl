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

GuiSize::GuiSize():
   mWidth(0),
   mHeight(0)
{
}

GuiSize::GuiSize(int width, int height):
   mWidth(width),
   mHeight(height)
{
}
GuiSize::GuiSize(const GuiSize& that):
   mWidth(that.mWidth),
   mHeight(that.mHeight)
{
}

INLINE int GuiSize::getWidth() const
{
   return mWidth;
}

INLINE void GuiSize::setWidth(int width)
{
   mWidth = width;
}

INLINE int GuiSize::getHeight() const
{
   return mHeight;
}

INLINE void GuiSize::setHeight(int height)
{
   mHeight = height;
}

INLINE void GuiRect::top(float t)
{
   _top = t;
}

INLINE float GuiRect::top() const
{
   return _top;
}

INLINE void GuiRect::left(float l)
{
   _left = l;
}

INLINE float GuiRect::left() const
{
   return _left;
}

INLINE void GuiRect::bottom(float b)
{
   _bottom = b;
}

INLINE float GuiRect::bottom() const
{
   return _bottom;
}

INLINE void GuiRect::right(float r)
{
   _right = r;
}

INLINE float GuiRect::right() const
{
   return _right;
}

INLINE void GuiRect::resize(float w, float h)
{
   _right = _left+w;
   _bottom = _top+h;
}

void GuiRect::set(float l, float r, float t, float b)
{
	_left = l;
   _right = r;
   _top = t;
   _bottom = b;
}

void GuiRect::offset(float x, float y)
{
	_left += x;
   _right += x;
   _top += y;
   _bottom += y;
}

bool GuiRect::pointInRect (const GuiPoint& point) const
{
	return (point.x >= _left && point.x <= _right && point.y >= _top && point.y <= _bottom);
}

float GuiRect::getWidth() const
{
	return _right - _left;
}

float GuiRect::getHeight() const
{
	return _bottom - _top;
}
