/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "../defines.h"

INLINE int TextureInfo::getWidth() const
{
   return mWidth;
}

INLINE void TextureInfo::setWidth(int width)
{
   mWidth = width;
}

INLINE int TextureInfo::getHeight() const
{
   return mHeight;
}

INLINE void TextureInfo::setHeight(int height)
{
   mHeight = height;
}

INLINE Uint8* TextureInfo::getData() const
{
   return mpData;
}

INLINE void TextureInfo::setData(Uint8* pdata)
{
   mpData = pdata;
}

INLINE int TextureInfo::getDataSize() const
{
   return mDataSize;
}

INLINE void TextureInfo::setDataSize(int size)
{
   mDataSize = size;
}

INLINE TextureInfo::Format TextureInfo::getFormat() const
{
   return mFormat;
}

void TextureInfo::setFormat(Format format)
{
   mFormat = format;
}