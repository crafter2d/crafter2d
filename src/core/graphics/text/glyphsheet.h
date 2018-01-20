
#ifndef GLYPH_SHEET_H
#define GLYPH_SHEET_H

#include <cstdint>
#include <vector>

#include "core/math/vector.h"

#include "glyphvertexdata.h"

namespace Graphics
{
   class Device;
   class Glyph;
   class GlyphVertexData;
   class RenderContext;
   class Texture;

   class GlyphSheet
   {
   public:
      GlyphSheet();
      ~GlyphSheet();

    // operations
      bool create(Device& device);

      uint32_t insertGlyph(const Glyph& glyph);
      void     flush(RenderContext& context);

      const GlyphVertexData& getGlyphVertexData(uint32_t glyphindex) const;
      const Texture&         getGlyphTexture() const;

   private:
      using VertexData = std::vector<GlyphVertexData>;

    // data
      VertexData  mCoords;
      uint8_t*    mpTextureData;
      Texture*    mpTexture;
      int         mTextureWidth;
      int         mTextureHeight;
      int         mLeft;
      int         mTop;
      bool        mDirty;
   };
}

#endif // GLYPH_SHEET_H
