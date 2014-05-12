
#include "textlayout.h"

#include <algorithm>
#include <vector>

#include "core/defines.h"
#include "core/graphics/font.h"
#include "core/math/vector.h"

#include "glyphatlas.h"
#include "glyphmap.h"
#include "glyphvertexdata.h"

namespace Graphics
{
   TextLayout::TextLayout():
      mpFont(NULL),
      mData(),
      mSortedData(),
      mGlyphIndices(),
      mVerticesPerSheet(),
      mMaxSheetIndex(0)
   {
   }

   // - Get/set

   Font& TextLayout::getFont()
   {
      ASSERT_PTR(mpFont);
      return *mpFont;
   }

   // - Operations

   bool TextLayout::create(RenderContext& context, const Vector& position, Font& font, float fontsize, const String& text)
   {
      mpFont = &font;

      GlyphAtlas& atlas = font.getGlyphAtlas();
      GlyphMap& map = atlas.getGlyphMap(fontsize);

      float positionx = position.x;
      mData.resize(text.length());
      mSortedData.resize(text.length());

      // first get all glyphs & determine number of sheets used
      mMaxSheetIndex = 1;
      for ( uint32_t index = 0; index < text.length(); ++index )
      {
         UChar ch = text[index];
         uint32_t glyphindex = atlas.getGlyph(map, ch);
         mGlyphIndices.push_back(glyphindex);

         const GlyphVertexData& data = atlas.getGlyphVertexData(glyphindex);
         TextLayoutData layoutdata;
         layoutdata.glyphindex = glyphindex;
         layoutdata.pos.set(positionx, position.y - data.mBaseLine);
         mData[index] = layoutdata;
         positionx += data.mAdvance;

         uint32_t sheet = glyphindex >> 16;
         mMaxSheetIndex = MAX(mMaxSheetIndex, sheet);
      }

      atlas.flush(context);

      sort();

      return true;
   }

   void TextLayout::sort()
   {
      // determine number of glyphs per sheet
      mVerticesPerSheet.resize(mMaxSheetIndex);
      for ( uint32_t index = 0; index < mGlyphIndices.size(); ++index )
      {
         uint32_t glyphindex = mGlyphIndices[index];
         uint32_t sheet = (glyphindex >> 16);
         ++mVerticesPerSheet[sheet];
      }

      // determine start index
      std::vector<uint32_t> startVertexPerSheet;
      startVertexPerSheet.resize(mMaxSheetIndex);
      uint32_t start = 0;
      for ( uint32_t index = 0; index < mMaxSheetIndex; ++index )
      {
         startVertexPerSheet[index] = start;
         start += mVerticesPerSheet[index];
      }

      // sort the layout data to efficiently render the glyphs per sheet
      for ( uint32_t index = 0; index < mData.size(); ++index )
      {
         TextLayoutData& data = mData[index];
         uint32_t sheet = (data.glyphindex >> 16);

         uint32_t& vertexIndex = startVertexPerSheet[sheet];
         mSortedData[vertexIndex] = data;
         ++vertexIndex;
      }
   }

   void TextLayout::fill(TextLayoutInfo& info)
   {
      info.sheetCount = mMaxSheetIndex;
      info.indicesPerSheet = &mVerticesPerSheet[0];
      info.dataCount = mSortedData.size();
      info.data = &mSortedData[0];
   }

      /*
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
      */

}
