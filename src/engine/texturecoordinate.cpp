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
#include "texturecoordinate.h"

TextureCoordinate::TextureCoordinate():
   mTL(),
   mBR()
{
}

TextureCoordinate::TextureCoordinate(const TextureCoordinate& that):
   mTL(that.mTL),
   mBR(that.mBR)
{
}

TextureCoordinate::~TextureCoordinate()
{
}

void TextureCoordinate::initialize(const Vector& tl, const Vector& br)
{
   mTL = tl;
   mBR = br;
}

Vector TextureCoordinate::getTopLeft()
{
   return mTL;
}

Vector TextureCoordinate::getTopRight()
{
   return Vector(mBR.x, mTL.y);
}

Vector TextureCoordinate::getBottomLeft()
{
   return Vector(mTL.x, mBR.y);
}

Vector TextureCoordinate::getBottomRight()
{
   return mBR;
}

void TextureCoordinate::flipHorizontal()
{
   float temp = mTL.x;
   mTL.x = mBR.x;
   mBR.x = temp;
}

void TextureCoordinate::flipVertical()
{
   float temp = mTL.y;
   mTL.y = mBR.y;
   mBR.y = temp;
}
