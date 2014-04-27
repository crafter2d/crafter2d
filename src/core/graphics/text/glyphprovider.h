
#ifndef GLYPH_PROVIDER_H
#define GLYPH_PROVIDER_H

#include <cstdint>

namespace Graphics
{
   class Font;
   class Glyph;

   /// GlyphProvider interface, should be implemented by a graphics device compliant
   /// module. It is used by the text engine to get the glyph bitmaps.

   class GlyphProvider
   {
   public:

      virtual bool initialize(int width, int height) = 0;
      virtual Glyph* getGlyph(Font& font, uint16_t glyphindex, float emsize) = 0;
   };
}

#endif // GLYPH_PROVIDER_H
