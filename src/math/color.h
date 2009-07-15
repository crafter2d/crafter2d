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
#ifndef COLOR_H_
#define COLOR_H_

class Color
{
public:
	Color();
   Color(const Color& color);
	Color(float red, float green, float blue, float alpha=1.0f);
	~Color();

 // get/set
   float getRed() const;
   void  setRed(float red);

   float getGreen() const;
   void  setGreen(float green);

   float getBlue() const;
   void  setBlue(float blue);

   float getAlpha() const;
   void  setAlpha(float alpha);

 // operations
	void set(float red, float green, float blue, float alpha=1.0f);

private:
	float mRed;
   float mGreen;
   float mBlue;
   float mAlpha;
};

#include "color.inl"

#endif
