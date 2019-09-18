
#include "glyphatlas.h"

#include <string.h>

#include "core/defines.h"

#include "../rendercontext.h"

#include "glyph.h"
#include "glyphsheet.h"

namespace Graphics
{
   GlyphAtlas::GlyphAtlas(Device& device) :
      mDevice(device),
      mSheets()
   {
   }

   GlyphAtlas::~GlyphAtlas()
   {
   }
 
   // - Operations

   uint32_t GlyphAtlas::insert(const Glyph& glyph)
   {
      uint32_t glyphindex = 0xffffff;
      int32_t sheetindex = 0;

      for (GlyphSheet& sheet : mSheets)
      {
         glyphindex = sheet.insertGlyph(glyph);
         if (glyphindex != 0xffffff)
         {
            break;
         }
         sheetindex++;
      }

      if (glyphindex == 0xffffff)
      {
         mSheets.emplace_back();
         auto& sheet = mSheets.back();
         sheet.create(mDevice);

         glyphindex = sheet.insertGlyph(glyph);
      }

      return (sheetindex << 16) | glyphindex;
   }

   const GlyphVertexData& GlyphAtlas::getGlyphVertexData(uint32_t glyphindex) const
   {
      uint32_t sheetindex = glyphindex >> 16;
      ASSERT(sheetindex != 0xfffff);
      return mSheets[sheetindex].getGlyphVertexData(glyphindex);
   }

   void GlyphAtlas::bind(RenderContext& context, uint32_t sheet) const
   {
      assert(sheet < mSheets.size());
      context.setTexture(0, mSheets[sheet].getGlyphTexture());
   }
   
   void GlyphAtlas::flush(RenderContext& context)
   {
      for (GlyphSheet& sheet : mSheets)
      {
         sheet.flush(context);
      }
   }
}
