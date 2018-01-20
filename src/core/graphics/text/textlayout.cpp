
#include "textlayout.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "core/defines.h"
#include "core/graphics/font.h"
#include "core/graphics/text/textrenderer.h"
#include "core/math/vector.h"

#include "glyphatlas.h"
#include "glyphmap.h"
#include "glyphvertexdata.h"

namespace Graphics
{
   TextLayout::TextLayout():
      mData(),
      mPosition(),
      mpTextRenderer(nullptr),
      mpFont(nullptr)
   {
   }

   TextLayout::TextLayout(TextRenderer& textrenderer):
      mData(),
      mPosition(),
      mpTextRenderer(&textrenderer),
      mpFont(nullptr)
   {
   }

   TextLayout::TextLayout(TextLayout&& other):
      mData(std::move(other.mData)),
      mPosition(other.mPosition),
      mpTextRenderer(other.mpTextRenderer),
      mpFont(other.mpFont)
   {
   }

   TextLayout& TextLayout::operator=(TextLayout&& other)
   {
      mData = std::move(other.mData);
      mpTextRenderer = other.mpTextRenderer;
      mpFont = other.mpFont;
      return *this;
   }

   // - Operations

   bool TextLayout::create(const Vector& position, Font& font, const String& text)
   {
      mpFont = &font;
      mPosition = position;
      mData.clear();

      float xpos = 0;
      float ypos = 0;
      const GlyphAtlas& atlas = mpTextRenderer->getGlyphAtlas();
     
      for ( uint32_t index = 0; index < text.length(); ++index )
      {
         UChar ch = text[index];
         uint32_t glyphindex = font.getGlyph(ch);
         uint32_t sheet = glyphindex >> 16;

         const GlyphVertexData& data = atlas.getGlyphVertexData(glyphindex);
         mData[sheet].emplace_back(glyphindex, Vector(xpos, position.y - data.mBaseLine));
         xpos += data.mAdvance;
      }
      
      return true;
   }
}
