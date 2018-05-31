
#include "oglglyphprovider.h"

#include FT_GLYPH_H

#include "core/graphics/text/glyph.h"
#include "core/defines.h"

#include "oglfont.h"

namespace Graphics
{
   OGLGlyphProvider::OGLGlyphProvider(FT_Library library, GlyphAtlas& atlas) :
      GlyphProvider(atlas),
      mLibrary(library),
      mpFont(nullptr)
   {
   }

   void OGLGlyphProvider::initialize(OGLFont& font)
   {
      mpFont = &font;
      mpFont->initialize(font.getSize());
   }

   uint32_t OGLGlyphProvider::getGlyph(UChar ch)
   {
      FT_Face face = mpFont->getFace();
      FT_GlyphSlot slot = face->glyph;

      int faceindex = FT_Get_Char_Index(face, ch);
      int error = FT_Load_Glyph(face, faceindex, FT_LOAD_DEFAULT);
      if ( error != 0 )
      {
         return 0xffffff;
      }

      FT_Glyph ftglyph;
      error = FT_Get_Glyph(slot, &ftglyph);
      error = FT_Glyph_To_Bitmap(&ftglyph, FT_RENDER_MODE_NORMAL, nullptr, true);
      if ( error != 0 )
      {
         FT_Done_Glyph(ftglyph);
         return 0xffffff;
      }

      FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)ftglyph;

      int width = bitmap_glyph->bitmap.width;
      int height = bitmap_glyph->bitmap.rows;

      uint32_t datasize = width * height;
      uint8_t* pdata = new uint8_t[datasize];
      memset(pdata, 0, datasize);

      uint8_t* psrc = bitmap_glyph->bitmap.buffer;
      uint8_t* pdest = pdata;

      for ( int h = 0; h < height; ++h )
      {
         memcpy(pdest, psrc, sizeof(uint8_t) * width);
         pdest += width;
         psrc += width;
         /*
         for ( int i = 0; i < width; i++ )
         {
            uint8_t pos = i + j * width;
            pdata[pos] = bitmap_glyph->bitmap.buffer[pos];
         }
         */
      }

      Glyph glyph;
      glyph.setSize(Size(static_cast<float>(width), static_cast<float>(height)));
      glyph.setAdvance(static_cast<float>(slot->advance.x >> 6));
      glyph.setBaseLine(static_cast<float>(bitmap_glyph->top));
      glyph.setPixels(pdata, width);
      uint32_t result = insertGlyph(glyph);

      FT_Done_Glyph(ftglyph);

      return result;
   }
}
