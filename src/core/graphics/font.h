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
#ifndef FONT_H_
#define FONT_H_

#include <unordered_map>

#include "core/string/string.h"
#include "core/core_base.h"

namespace Graphics
{
   class GlyphProvider;

   class CORE_API Font
   {
   public:
      explicit Font(GlyphProvider* pprovider);
      virtual ~Font();

    // get/set
      const String& getFamilyName() const {
         return mFamilyName;
      }

      void setFamilyName(const String& name) {
         mFamilyName = name;
      }

      float getSize() const {
         return mSize;
      }
      void setSize(float size) {
         mSize = size;
      }

      uint32_t getGlyph(UChar ch);
      
    // query
      virtual int      getBaseLine() const = 0;
      
   private:
      using GlyphMap = std::unordered_map<UChar, uint32_t>;

    // data
      GlyphProvider* mpGlyphProvider;
      GlyphMap       mGlyphMap;
      String         mFamilyName;
      float          mSize;
   };
};

#endif // FONT_H_
