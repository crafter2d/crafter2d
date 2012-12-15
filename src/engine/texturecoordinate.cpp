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
   mCoordinates()
{
}

TextureCoordinate::TextureCoordinate(const TextureCoordinate& that):
   mCoordinates()
{
   mCoordinates[TopLeft] = that.getTopLeft();
   mCoordinates[TopRight] = that.getTopRight();
   mCoordinates[BottomRight] = that.getBottomRight();
   mCoordinates[BottomLeft] = that.getBottomLeft();
}

void TextureCoordinate::initialize(const Vertex& tl, const Vertex& br)
{
   mCoordinates[TopLeft] =  tl;
   mCoordinates[TopRight].set(br.x, tl.y);
   mCoordinates[BottomRight] = br;
   mCoordinates[BottomLeft].set(tl.x, br.y);
}

const Vertex& TextureCoordinate::getTopLeft() const
{
   return mCoordinates[TopLeft];
}

const Vertex& TextureCoordinate::getTopRight() const
{
   return mCoordinates[TopRight];
}

const Vertex& TextureCoordinate::getBottomLeft() const
{
   return mCoordinates[BottomRight];
}

const Vertex& TextureCoordinate::getBottomRight() const
{
   return mCoordinates[BottomLeft];
}
