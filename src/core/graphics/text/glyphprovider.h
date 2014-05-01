
#ifndef GLYPH_PROVIDER_H
#define GLYPH_PROVIDER_H

#include <cstdint>
#include "core/string/string.h"

namespace Graphics
{
   class Font;
   class Glyph;

   /// GlyphProvider interface, should be implemented by a graphics device compliant
   /// module. It is used by the text engine to get the glyph bitmaps.

   class GlyphProvider
   {
   public:

      virtual Glyph* getGlyph(UChar ch, float emsize) = 0;
   };
}

#endif // GLYPH_PROVIDER_H
