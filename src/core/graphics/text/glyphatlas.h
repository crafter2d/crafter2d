
#ifndef GLYPH_ATLAS_H
#define GLYPH_ATLAS_H

#include <cstdint>

namespace Graphics
{
   class Glyph;
   class GlyphSheet;

   class GlyphAtlas
   {
   public:
      GlyphAtlas(int maxsheets);

    // operations
      uint32_t insertGlyph(const Glyph& glyph);

   private:

    // data
      GlyphSheet** mpSheets;
      int          mMaxSheets;
      int          mCurSheets;
   };
}

#endif // GLYPH_ATLAS_H
