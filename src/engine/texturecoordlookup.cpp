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
#include "texture.h"

#include "core/math/vector.h"
#include "core/defines.h"

#include "texturecoordlookup.h"

TextureCoordLookup::TextureCoordLookup():
   _lookupTable(NULL),
   _texFrameWidth(0.0f),
   _texFrameHeight(0.0f),
   _frameCount(0)
{
}

TextureCoordLookup::~TextureCoordLookup()
{
   delete[] _lookupTable;
   _lookupTable = NULL;
}

void TextureCoordLookup::generateFromTexture(const Texture& texture, float framewidth, float frameheight, int framecount)
{
   _frameCount = framecount;

   int maxFramesPerRow = texture.getWidth() / framewidth;
   float nX = static_cast<float>(texture.getWidth()) / framewidth;
   float nY = static_cast<float>(texture.getHeight()) / frameheight;

   _texFrameWidth  = texture.getSourceWidth() / nX;
   _texFrameHeight = texture.getSourceHeight() / nY;

   // build the texture coord lookup table
	_lookupTable = new TextureCoordinate[_frameCount];
	for (int tc = 0; tc < _frameCount; tc++)
   {
		// calculate starting texture coordinates
      float x = static_cast<float>(tc % maxFramesPerRow) * _texFrameWidth;
      float y = floorf ((float)tc / maxFramesPerRow) * _texFrameHeight;

      Vector tl = Vector(x, y);
      Vector br = Vector(x + _texFrameWidth, y + _texFrameHeight);

      _lookupTable[tc].initialize(tl, br);
	}
}

const TextureCoordinate& TextureCoordLookup::operator[](int index) const
{
   ASSERT(index < _frameCount);

   return _lookupTable[index];
}

float TextureCoordLookup::getFrameWidth() const
{
   return _texFrameWidth;
}

float TextureCoordLookup::getFrameHeight() const
{
   return _texFrameHeight;
}
