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
#include "guihelpers.h"
#ifndef JENGINE_INLINE
#  include "guihelpers.inl"
#endif

GuiPoint::GuiPoint():
   x(0),
   y(0)
{
}

GuiPoint::GuiPoint(int _x, int _y):
   x(_x),
   y(_y)
{
}

GuiPoint::GuiPoint(const GuiPoint& p):
   x(p.x),
   y(p.y)
{
}

GuiPoint::GuiPoint(const Point& p):
   x(p.x()),
   y(p.y())
{
}

GuiColor mWhite   = GuiColor(1.0f, 1.0f, 1.0f, 1.0f);
GuiColor mBlack   = GuiColor(0.0f, 0.0f, 0.0f, 1.0f);
GuiColor mRed     = GuiColor(1.0f, 0.0f, 0.0f, 1.0f);
GuiColor mGreen   = GuiColor(0.0f, 1.0f, 0.0f, 1.0f);
GuiColor mBlue    = GuiColor(0.0f, 0.0f, 1.0f, 1.0f);

const GuiColor& GuiColor::white()
{
   return mWhite;
}

const GuiColor& GuiColor::black()
{
   return mBlack;
}

const GuiColor& GuiColor::red()
{
   return mRed;
}

const GuiColor& GuiColor::green()
{
   return mGreen;
}

const GuiColor& GuiColor::blue()
{
   return mBlue;
}

GuiColor::GuiColor():
   r(1),
   g(1),
   b(1),
   a(1)
{
}

GuiColor::GuiColor(float _r, float _g, float _b, float _a):
   r(_r),
   g(_g),
   b(_b),
   a(_a)
{
}

GuiColor::GuiColor(int _r, int _g, int _b, int _a):
   r(_r/255.0f),
   g(_g/255.0f),
   b(_b/255.0f),
   a(_a/255.0f)
{
}

GuiRect::GuiRect():
   _left(0),
   _right(0),
   _top(0),
   _bottom(0)
{
}

GuiRect::GuiRect(const GuiRect& rect): 
   _left(rect._left),
   _right(rect._right),
   _top(rect._top),
   _bottom(rect._bottom)
{
}

GuiRect::GuiRect(float l, float r, float t, float b):
	_left(l),
   _right(r),
   _top(t),
   _bottom(b)
{
}

void GuiRect::adjustBase (float width, float height)
{
	_left = (_left * width) / 4;
	_right = (_right * width) / 4;
	_top = (_top * height) / 8;
	_bottom = (_bottom * height) / 8;
}

