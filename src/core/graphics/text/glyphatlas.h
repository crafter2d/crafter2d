
#ifndef GLYPH_ATLAS_H
#define GLYPH_ATLAS_H

#include <map>
#include <cstdint>

#include "core/string/string.h"

#include "glyphsheet.h"

namespace Graphics
{
   class Device;
   class Glyph;
   class GlyphVertexData;
   class RenderContext;
   class Texture;

   class GlyphAtlas final
   {
   public:
      explicit GlyphAtlas(Device& device);
      ~GlyphAtlas();
            
    // operations
      uint32_t insert(const Glyph& glyph);
      void bind(RenderContext& context, uint32_t sheet) const;
      void flush(RenderContext& context);

      const GlyphVertexData& getGlyphVertexData(uint32_t glyphindex) const;

   private:
      using GlyphSheets = ::std::vector<GlyphSheet>;
            
    // data
      Device&        mDevice;
      GlyphSheets    mSheets;
   };
}

#endif // GLYPH_ATLAS_H
