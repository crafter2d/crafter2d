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
#ifndef GUIHELPERS_H_
#define GUIHELPERS_H_

#include "../math/point.h"

class GuiPoint
{
public:
   GuiPoint();
   GuiPoint(int _x, int _y);
   GuiPoint(const GuiPoint& p);
   GuiPoint(const Point& p);

   operator Point() const;

   int x, y;
};

class GuiSize
{
public:
   GuiSize();
   GuiSize(int width, int height);
   GuiSize(const GuiSize& that);

   int   getWidth() const;
   void  setWidth(int width);

   int   getHeight() const;
   void  setHeight(int height);

private:
   int mWidth;
   int mHeight;
};

class GuiColor
{
public:
   static const GuiColor&  white();
   static const GuiColor&  black();
   static const GuiColor&  red();
   static const GuiColor&  green();
   static const GuiColor&  blue();

         GuiColor();
         GuiColor(float _r, float _g, float _b, float _a=1.0f);
         GuiColor(int _r, int _g, int _b, int _a=255);

   void  set(float _r, float _g, float _b, float _a=1.0f);

   float r,g,b,a;
};

class GuiRect
{
public:
	      GuiRect();
         GuiRect(const GuiRect& rect);
	      GuiRect(float l, float r, float t, float b);
	
	void  set(float l, float r, float t, float b);
	void  offset(float x, float y);
   void  resize(float w, float h);
	void  adjustBase (float width, float height);
	bool  pointInRect (const GuiPoint& point) const;

   void  top(float t);
   float top() const;

   void  left(float l);
   float left() const;

   void  bottom(float b);
   float bottom() const;

   void  right(float r);
   float right() const;
	
	float getWidth() const;
	float getHeight() const;

private:
	float _left, _right;
	float _top, _bottom;
};

#ifdef JENGINE_INLINE
#  include "guihelpers.inl"
#endif

#endif

