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

INLINE const String& Texture::getName() const
{
   return mName;
}

INLINE void Texture::setName(const String& name)
{
   mName = name;
}

INLINE const String& Texture::getUniform() const
{
   return mUniform;
}
      
INLINE void Texture::setUniform(const String& uniform)
{
   mUniform = uniform;
}

INLINE int Texture::getStage() const
{
   return mTexStage;
}

INLINE void Texture::setStage(int stage)
{
   mTexStage = stage;
}

INLINE int Texture::getWidth() const
{
   return _width; 
}

INLINE int Texture::getHeight() const
{
   return _height;
}

INLINE float Texture::getSourceWidth() const
{
   return _sourceWidth;
}

INLINE float Texture::getSourceHeight() const
{
   return _sourceHeight;
}

};