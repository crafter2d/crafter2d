
#include "glyphprovider.h"

#include "glyphatlas.h"

namespace Graphics
{
   uint32_t GlyphProvider::insertGlyph(const Glyph& glyph)
   {
      return mAtlas.insert(glyph);
   }
}
