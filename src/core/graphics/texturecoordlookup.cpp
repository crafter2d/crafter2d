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
#include "texturecoordlookup.h"

#include "core/math/size.h"
#include "core/math/vertex.h"
#include "core/defines.h"

#include "texture.h"

namespace Graphics
{

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

// - Query
      
bool TextureCoordLookup::isEmpty() const
{
   return _frameCount == 0;
}

// - Operations

void TextureCoordLookup::generateFromTexture(const Graphics::Texture& texture, const Size& framesize, int framecount)
{
   _frameCount = framecount;

   int maxFramesPerRow = static_cast<int>(texture.getWidth() / framesize.width);
   float nX = static_cast<float>(texture.getWidth()) / framesize.width;
   float nY = static_cast<float>(texture.getHeight()) / framesize.height;

   _texFrameWidth  = texture.getSourceWidth() / nX;
   _texFrameHeight = texture.getSourceHeight() / nY;

   // build the texture coord lookup table
	_lookupTable = new TextureCoordinate[_frameCount];
	for (int tc = 0; tc < _frameCount; tc++)
   {
		// calculate starting texture coordinates
      float x = static_cast<float>(tc % maxFramesPerRow) * _texFrameWidth;
      float y = floorf ((float)tc / maxFramesPerRow) * _texFrameHeight;

      Vertex tl(x, y);
      Vertex br(x + _texFrameWidth, y + _texFrameHeight);

      _lookupTable[tc].initialize(tl, br);
	}
}

void TextureCoordLookup::flip()
{
   for ( int index = 0; index < _frameCount; ++index )
   {
      _lookupTable[index].flip();
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

} // namespace Graphics
