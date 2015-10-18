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

inline Color::Color():
   mRed(0.0f),
   mGreen(0.0f),
   mBlue(0.0f),
   mAlpha(1.0f)
{
}

inline Color::Color(const Color& color):
   mRed(color.mRed),
   mGreen(color.mGreen),
   mBlue(color.mBlue),
   mAlpha(color.mAlpha)
{
}

inline Color::Color(int red, int green, int blue, int alpha):
   mRed(red / 255.0f),
   mGreen(green / 255.0f),
   mBlue(blue / 255.0f),
   mAlpha(alpha / 255.0f)
{
}

inline Color::Color(float red, float green, float blue, float alpha):
   mRed(red),
   mGreen(green),
   mBlue(blue),
   mAlpha(alpha)
{
}

inline Color::~Color()
{
}

inline Color& Color::operator=(const Color& that)
{
   mRed = that.mRed;
   mGreen = that.mGreen;
   mBlue = that.mBlue;
   mAlpha = that.mAlpha;
   return *this;
}

inline void Color::set(float red, float green, float blue, float alpha) 
{
	mRed   = red;
   mGreen = green;
   mBlue  = blue;
   mAlpha = alpha;
}
