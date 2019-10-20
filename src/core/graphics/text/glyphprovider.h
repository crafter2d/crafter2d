
#ifndef GLYPH_PROVIDER_H
#define GLYPH_PROVIDER_H

#include <cstdint>
#include "core/string/string.h"

namespace Graphics
{
   class Font;
   class Glyph;
   class GlyphAtlas;

   /// GlyphProvider interface, should be implemented by a graphics device compliant
   /// module. It is used by the text engine to get the glyph bitmaps.

   class CORE_API GlyphProvider
   {
   public:
      GlyphProvider(GlyphAtlas& atlas) :
         mAtlas(atlas)
      {
      }

      virtual ~GlyphProvider() = default;

      virtual uint32_t getGlyph(UChar ch) = 0;

   protected:
   // operations
      uint32_t insertGlyph(const Glyph& glyph);

   private:

      GlyphAtlas& mAtlas;
   };
}

#endif // GLYPH_PROVIDER_H
