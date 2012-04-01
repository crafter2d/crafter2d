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
#ifndef _TEXTURE_COORD_LOOKUP
#define _TEXTURE_COORD_LOOKUP

#include "texturecoordinate.h"

class Texture;
class Vector;

class TextureCoordLookup
{
public:
   TextureCoordLookup();
   ~TextureCoordLookup();

 // Get/set interface
   float getFrameWidth() const;
   float getFrameHeight() const;

 // Operations
   void generateFromTexture(const Texture& texture, float framewidth, float frameheight, int framecount);
   
   const TextureCoordinate& operator[](int index) const;

private:
   TextureCoordinate* _lookupTable;
   float _texFrameWidth;
   float _texFrameHeight;
   int _frameCount;
};

#endif
