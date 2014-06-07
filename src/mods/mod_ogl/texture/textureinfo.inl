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

namespace Graphics
{

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

INLINE uint8_t* TextureInfo::getData() const
{
   return mpData;
}

INLINE void TextureInfo::setData(uint8_t* pdata)
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

INLINE int TextureInfo::getChannels() const
{
   return mChannels;
}

void TextureInfo::setChannels(int channels)
{
   mChannels = channels;
}

};
