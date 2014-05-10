
#include "textlayout.h"

#include <algorithm>
#include <vector>

#include "core/graphics/font.h"
#include "core/math/vector.h"

#include "glyphatlas.h"
#include "glyphmap.h"
#include "glyphvertexdata.h"

namespace Graphics
{
   TextLayout::TextLayout()
   {
   }

   // - Operations

   bool glyphSort(uint32_t i, uint32_t j) { return (i >> 16) < (j >> 16); }

   bool TextLayout::create(RenderContext& context, const Vector& position, Font& font, float fontsize, const String& text)
   {
      GlyphAtlas& atlas = font.getGlyphAtlas();
      GlyphMap& map = atlas.getGlyphMap(fontsize);

      std::vector<uint32_t> indices;
      for ( uint32_t index = 0; index < text.length(); ++index )
      {
         UChar ch = text[index];
         uint32_t glyphindex = atlas.getGlyph(map, ch);
         indices.push_back(glyphindex);
      }

      atlas.flush(context);

      mCount = indices.size() * 4;
      mpVertices = new GlyphVertex[mCount];

      float positionx = position.x;
      float positiony = position.y;

      GlyphVertex* pvertices = mpVertices;
      for ( uint32_t index = 0; index < indices.size(); ++index )
      {
         uint32_t glyphindex = indices[index];

         const GlyphVertexData& data = atlas.getGlyphVertexData(glyphindex);
         pvertices[0].pos.x = positionx;
         pvertices[0].pos.y = positiony;
         pvertices[0].tex.x = data.mTexturePos.x;
         pvertices[0].tex.y = data.mTexturePos.y;

         pvertices[1].pos.x = positionx + data.mGlyphSize.x;
         pvertices[1].pos.y = positiony;
         pvertices[1].tex.x = data.mTexturePos.x + data.mTextureDim.x;
         pvertices[1].tex.y = data.mTexturePos.y;

         pvertices[2].pos.x = positionx + data.mGlyphSize.x;
         pvertices[2].pos.y = positiony + data.mGlyphSize.y;
         pvertices[2].tex.x = data.mTexturePos.x + data.mTextureDim.x;
         pvertices[2].tex.y = data.mTexturePos.y + data.mTextureDim.y;

         pvertices[3].pos.x = positionx;
         pvertices[3].pos.y = positiony + data.mGlyphSize.y;
         pvertices[3].tex.x = data.mTexturePos.x;
         pvertices[3].tex.y = data.mTexturePos.y + data.mTextureDim.y;

         positionx += data.mGlyphAdvance;
         pvertices += 4;
      }

      return true;
   }
}
