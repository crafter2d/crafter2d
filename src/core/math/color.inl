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

inline float Color::getRed() const
{
   return mRed;
}

inline void Color::setRed(float red)
{
   mRed = red;
}

inline float Color::getGreen() const
{
   return mGreen;
}

inline void Color::setGreen(float green)
{
   mGreen = green;
}

inline float Color::getBlue() const
{
   return mBlue;
}

inline void Color::setBlue(float blue)
{
   mBlue = blue;
}

inline float Color::getAlpha() const
{
   return mAlpha;
}

inline void Color::setAlpha(float alpha)
{
   mAlpha = alpha;
}

inline void Color::set(float red, float green, float blue, float alpha) 
{
	mRed   = red;
   mGreen = green;
   mBlue  = blue;
   mAlpha = alpha;
}