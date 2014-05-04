
#ifndef GLYPH_SHEET_H
#define GLYPH_SHEET_H

#include <cstdint>
#include <vector>

#include "core/math/vector.h"

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
      struct GlyphCoord
      {
         Vector pos;
         Vector size;
      };

      typedef std::vector<GlyphCoord> Coords;

    // data
      uint8_t* mpTextureData;
      Texture* mpTexture;
      Coords   mCoords;
      int      mTextureWidth;
      int      mTextureHeight;
      int      mTop;
      int      mLeft;
   };
}

#endif // GLYPH_SHEET_H
