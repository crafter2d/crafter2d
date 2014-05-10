
#ifndef GLYPH_ATLAS_H
#define GLYPH_ATLAS_H

#include <map>
#include <cstdint>

#include "core/string/string.h"

namespace Graphics
{
   class Device;
   class Glyph;
   class GlyphMap;
   class GlyphProvider;
   class GlyphSheet;
   class GlyphVertexData;
   class RenderContext;
   class Texture;

   class GlyphAtlas
   {
   public:
      GlyphAtlas(Device& device, GlyphProvider* pprovider, int maxsheets);

    // get/set
      GlyphProvider& getProvider();

    // query
      GlyphMap& getGlyphMap(float size);

    // operations
      uint32_t getGlyph(GlyphMap& map, UChar ch);
      void flush(RenderContext& context);

      const GlyphVertexData& getGlyphVertexData(uint32_t glyphindex) const;
      const Texture&         getGlyphTexture(uint32_t glyphindex) const;

   private:
      typedef std::map<float, GlyphMap*> GlyphMaps;

    // operations
      uint32_t insertGlyph(const Glyph& glyph);
      
    // data
      Device&        mDevice;
      GlyphProvider* mpProvider;
      GlyphMaps      mGlyphMaps;
      GlyphSheet**   mpSheets;
      int            mMaxSheets;
      int            mCurSheets;
   };
}

#endif // GLYPH_ATLAS_H
