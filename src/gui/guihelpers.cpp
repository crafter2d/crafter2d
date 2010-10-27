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

