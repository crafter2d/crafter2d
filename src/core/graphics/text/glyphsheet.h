
#ifndef GLYPH_SHEET_H
#define GLYPH_SHEET_H

#include <cstdint>

namespace Graphics
{
   class Glyph;
   class Texture;

   class GlyphSheet
   {
   public:
      GlyphSheet();

    // operations
      uint32_t insertGlyph(const Glyph& glyph);

   private:

    // data
      uint8_t* mpTextureData;
      Texture* mpTexture;
   };
}

#endif // GLYPH_SHEET_H
