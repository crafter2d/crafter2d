
#ifndef GLYPH_MAP_H
#define GLYPH_MAP_H

#include <map>

#include "core/string/string.h"

namespace Graphics
{
   class Glyph;

   class GlyphMap
   {
      using GlyphLookup = ::std::map<UChar, uint32_t>;

   public:
      explicit GlyphMap(float fontsize) : mFontSize(fontsize), mLookup() {}

    // get/set
      inline float getFontSize() const { return mFontSize; }

    // operations
      void insert(UChar ch, uint32_t glyphindex) {
         mLookup.insert(std::make_pair(ch, glyphindex));
      }

      inline uint32_t lookup(UChar ch) {
         GlyphLookup::iterator it = mLookup.find(ch);
         return it != mLookup.end() ? it->second : 0xffffff;
      }

   private:

    //data
      GlyphLookup mLookup;
      float       mFontSize;
   };
}

#endif // GLYPH_MAP_H
