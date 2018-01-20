/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#include "font.h"

#include "core/defines.h"

#include "text/glyph.h"
#include "text/glyphprovider.h"

namespace Graphics
{

   Font::Font(GlyphProvider* pprovider) :
      mpGlyphProvider(pprovider),
      mGlyphMap(0),
      mFamilyName(),
      mSize(0.0f)
   {
      assert(pprovider != nullptr);
   }

   Font::~Font()
   {
      delete mpGlyphProvider;
   }

   // - Get/set
   
   uint32_t Font::getGlyph(UChar ch)
   {
      auto it = mGlyphMap.find(ch);
      if (it != mGlyphMap.end())
      {
         return it->second;
      }
      
      uint32_t glyphindex = mpGlyphProvider->getGlyph(ch);
      if ( glyphindex != 0xffffff )
      {
         mGlyphMap.emplace(ch, glyphindex);
      }
      return glyphindex;
   }

}
